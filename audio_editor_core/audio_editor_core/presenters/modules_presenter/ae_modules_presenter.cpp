//
//  ae_modules_presenter.cpp
//

#include "ae_modules_presenter.h"

#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_module_content_pool.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_track_editor.h>
#include <cpp_utils/yas_stl_utils.h>

#include <audio_editor_core/ae_selected_file_module_pool.hpp>

using namespace yas;
using namespace yas::ae;

static std::string const empty_string = "";
static std::vector<std::optional<module_content>> const _empty_contents;

std::shared_ptr<modules_presenter> modules_presenter::make_shared(window_lifetime_id const &window_lifetime_id,
                                                                  std::shared_ptr<display_space> const &display_space) {
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(window_lifetime_id);
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    return std::make_shared<modules_presenter>(
        project_lifetime->project_format, window_lifetime->player, project_lifetime->file_track,
        project_lifetime->selected_file_module_pool, display_space, project_lifetime->module_content_pool);
}

modules_presenter::modules_presenter(project_format const &project_format, std::shared_ptr<player> const &player,
                                     std::shared_ptr<file_track> const &file_track,
                                     std::shared_ptr<selected_file_module_pool> const &selected_pool,
                                     std::shared_ptr<display_space> const &display_space,
                                     std::shared_ptr<module_content_pool> const &content_pool)
    : _project_format(project_format),
      _player(player),
      _file_track(file_track),
      _selected_pool(selected_pool),
      _display_space(display_space),
      _content_pool(content_pool) {
    auto const sample_rate = this->_project_format.sample_rate;

    file_track
        ->observe_event([this, sample_rate](file_track_event const &event) {
            switch (event.type) {
                case file_track_event_type::any:
                case file_track_event_type::reverted:
                    // 初期状態やアンドゥした時に呼ばれる
                    // とにかく強制的に全部更新する
                    this->_update_all_contents(true, true);
                    break;
                case file_track_event_type::erased: {
                    this->_erase_content(event.module.value().identifier);
                } break;
                case file_track_event_type::inserted: {
                    this->_insert_content(event.module.value());
                } break;
                case file_track_event_type::detail_updated: {
                    this->_replace_content(event.module.value().index());
                } break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);

    selected_pool
        ->observe_event([this, sample_rate](selected_file_module_pool::event const &event) {
            using event_type = selected_file_module_pool::event_type;

            switch (event.type) {
                case event_type::any:
                    this->_update_all_contents(true, false);
                    break;
                case event_type::inserted:
                case event_type::erased: {
                    this->_replace_content(event.module.value().index());
                } break;
            }
        })
        .end()
        ->add_to(this->_canceller_pool);

    display_space
        ->observe([this](display_space_event const &event) {
            // 画面サイズが変わった時に呼ばれる
            // スケールや再生位置に関係なく更新する（？）。各モジュールは変わらない想定
            this->_update_all_contents(true, false);
        })
        .sync()
        ->add_to(this->_canceller_pool);
}

std::vector<std::optional<module_content>> const &modules_presenter::contents() const {
    if (auto const content_pool = this->_content_pool.lock()) {
        return content_pool->elements();
    } else {
        return _empty_contents;
    }
}

observing::syncable modules_presenter::observe_contents(
    std::function<void(module_content_pool_event const &)> &&handler) {
    if (auto const content_pool = this->_content_pool.lock()) {
        return content_pool->observe_event(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

std::string const &modules_presenter::name_for_index(file_module_index const &index) {
    if (auto const file_track = this->_file_track.lock()) {
        if (file_track->modules().contains(index)) {
            return file_track->modules().at(index).value.name;
        }
    }

    return empty_string;
}

void modules_presenter::update_if_needed() {
    // 毎フレーム呼ばれて、再生位置が変わっていたら更新する
    this->_update_all_contents(false, false);
}

std::optional<time::range> modules_presenter::_space_range() const {
    auto const player = this->_player.lock();
    auto const display_space = this->_display_space.lock();
    if (player && display_space) {
        auto const sample_rate = this->_project_format.sample_rate;
        auto const current_frame = player->current_frame();
        return display_space->frame_range(sample_rate, current_frame);
    } else {
        return std::nullopt;
    }
}

void modules_presenter::_insert_content(file_module_object const &module) {
    auto const content_pool = this->_content_pool.lock();
    auto const display_space = this->_display_space.lock();
    auto const selected_pool = this->_selected_pool.lock();

    if (!content_pool || !display_space || !selected_pool) {
        return;
    }

    auto const space_range = this->_space_range();
    if (space_range.has_value() && module.value.range.is_overlap(space_range.value())) {
        content_pool->insert({module, selected_pool->contains(module.index()), this->_project_format.sample_rate,
                              space_range.value(), display_space->scale().width});
    }
}

void modules_presenter::_erase_content(object_id const &object_id) {
    auto const content_pool = this->_content_pool.lock();

    if (!content_pool) {
        return;
    }

    content_pool->erase_for_id(object_id);
}

void modules_presenter::_replace_content(file_module_index const &index) {
    auto const content_pool = this->_content_pool.lock();
    auto const display_space = this->_display_space.lock();
    auto const file_track = this->_file_track.lock();
    auto const selected_pool = this->_selected_pool.lock();

    if (!content_pool || !display_space || !file_track || !selected_pool) {
        return;
    }

    auto const space_range = this->_space_range();
    if (space_range.has_value() && index.range.is_overlap(space_range.value()) &&
        file_track->modules().contains(index)) {
        auto const &module = file_track->modules().at(index);
        content_pool->replace({module, selected_pool->contains(index), this->_project_format.sample_rate,
                               space_range.value(), display_space->scale().width});
    }
}

// force_updating -> スケールや再生位置が変わっていなくても強制的に更新するか
// force_replacing -> 要素が変わっていなくても強制的に更新するか
void modules_presenter::_update_all_contents(bool const force_updating, bool const force_replacing) {
    auto const space_range = this->_space_range();
    auto const player = this->_player.lock();
    auto const file_track = this->_file_track.lock();
    auto const display_space = this->_display_space.lock();
    auto const content_pool = this->_content_pool.lock();
    auto const selected_pool = this->_selected_pool.lock();

    if (player && file_track && space_range.has_value() && display_space && content_pool && selected_pool) {
        auto const current_frame = player->current_frame();

        if (space_range == this->_last_space_range && current_frame == this->_last_frame && !force_updating) {
            return;
        }

        auto const &space_range_value = space_range.value();
        auto const scale = display_space->scale().width;

        auto const contents = filter_map<module_content>(
            file_track->modules(), [&space_range_value, sample_rate = this->_project_format.sample_rate, &scale,
                                    &selected_pool](auto const &module) {
                if (module.first.range.is_overlap(space_range_value)) {
                    return std::make_optional<module_content>(module.second,
                                                              selected_pool->contains(module.second.index()),
                                                              sample_rate, space_range_value, scale);
                } else {
                    return std::optional<module_content>(std::nullopt);
                }
            });

        content_pool->update_all(contents, force_replacing);

        this->_last_frame = current_frame;
        this->_last_space_range = space_range;
    }
}
