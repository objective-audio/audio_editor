//
//  ae_modules_presenter.cpp
//

#include "ae_modules_presenter.h"

#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_module_content_pool.h>
#include <audio_editor_core/ae_module_editor.h>
#include <audio_editor_core/ae_module_pool.h>
#include <audio_editor_core/ae_player.h>
#include <cpp_utils/yas_lock.h>
#include <cpp_utils/yas_stl_utils.h>

#include <audio_editor_core/ae_display_space_range.hpp>
#include <audio_editor_core/ae_range_selector.hpp>

using namespace yas;
using namespace yas::ae;

static std::string const empty_string = "";
static std::vector<std::optional<module_content>> const _empty_contents;

std::shared_ptr<modules_presenter> modules_presenter::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(project_lifetime_id);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    return std::make_shared<modules_presenter>(
        project_editing_lifetime->project_format, window_lifetime->player, project_editing_lifetime->module_pool,
        project_editing_lifetime->selected_module_pool, window_lifetime->display_space,
        project_editing_lifetime->display_space_range, project_editing_lifetime->module_content_pool,
        project_editing_lifetime->range_selector);
}

modules_presenter::modules_presenter(project_format const &project_format, std::shared_ptr<player> const &player,
                                     std::shared_ptr<module_pool> const &module_pool,
                                     std::shared_ptr<selected_module_pool> const &selected_pool,
                                     std::shared_ptr<display_space> const &display_space,
                                     std::shared_ptr<display_space_range> const &display_space_range,
                                     std::shared_ptr<module_content_pool> const &content_pool,
                                     std::shared_ptr<range_selector> const &range_selector)
    : _project_format(project_format),
      _player(player),
      _module_pool(module_pool),
      _selected_pool(selected_pool),
      _display_space(display_space),
      _display_space_range(display_space_range),
      _content_pool(content_pool),
      _range_selector(range_selector) {
    auto const sample_rate = this->_project_format.sample_rate;

    module_pool
        ->observe_event([this, sample_rate](module_pool_event const &event) {
            switch (event.type) {
                case module_pool_event_type::any:
                case module_pool_event_type::reverted:
                    // 初期状態やアンドゥした時に呼ばれる
                    // とにかく強制的に全部更新する
                    this->_update_all_contents(true, true);
                    break;
                case module_pool_event_type::erased: {
                    this->_erase_content(event.module.value().identifier);
                } break;
                case module_pool_event_type::inserted: {
                    this->_insert_content(event.module.value());
                } break;
                case module_pool_event_type::detail_updated: {
                    this->_replace_contents({event.module.value().index()});
                } break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);

    selected_pool
        ->observe_event([this, sample_rate](selected_module_pool::event const &event) {
            using event_type = selected_pool_event_type;

            switch (event.type) {
                case event_type::fetched:
                    this->_update_all_contents(true, false);
                    break;
                case event_type::inserted:
                case event_type::erased: {
                    auto const indices =
                        to_vector<module_index>(event.elements, [](auto const &pair) { return pair.first; });
                    this->_replace_contents(indices);
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

observing::syncable modules_presenter::observe_range_selection_region(
    std::function<void(std::optional<ui::region> const &)> &&handler) {
    if (auto const range_selector = this->_range_selector.lock()) {
        return range_selector->observe_region(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

std::string modules_presenter::name_for_index(module_index const &index) {
    if (auto const module_pool = this->_module_pool.lock()) {
        if (auto const module = module_pool->module_at(index)) {
            return module.value().value.name;
        }
    }

    return empty_string;
}

void modules_presenter::update_if_needed() {
    // 毎フレーム呼ばれて、再生位置が変わっていたら更新する
    this->_update_all_contents(false, false);
}

std::optional<time::range> modules_presenter::_space_range() const {
    if (auto const space_range = this->_display_space_range.lock()) {
        return space_range->current();
    } else {
        return std::nullopt;
    }
}

void modules_presenter::_insert_content(module_object const &module) {
    auto const locked = yas::lock(this->_content_pool, this->_display_space, this->_selected_pool);

    if (!fulfilled(locked)) {
        return;
    }

    auto const &[content_pool, display_space, selected_pool] = locked;

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

void modules_presenter::_replace_contents(std::vector<module_index> const &indices) {
    auto const locked = yas::lock(this->_content_pool, this->_display_space, this->_module_pool, this->_selected_pool);

    if (!fulfilled(locked)) {
        return;
    }

    auto const &[content_pool, display_space, module_pool, selected_pool] = locked;

    auto const space_range = this->_space_range();
    if (space_range.has_value()) {
        for (auto const &index : indices) {
            if (index.range.is_overlap(space_range.value())) {
                if (auto const module = module_pool->module_at(index)) {
                    content_pool->replace({module.value(), selected_pool->contains(index),
                                           this->_project_format.sample_rate, space_range.value(),
                                           display_space->scale().width});
                }
            }
        }
    }
}

// force_updating -> スケールや再生位置が変わっていなくても強制的に更新するか
// force_replacing -> 要素が変わっていなくても強制的に更新するか
void modules_presenter::_update_all_contents(bool const force_updating, bool const force_replacing) {
    auto const space_range = this->_space_range();
    auto const locked =
        yas::lock(this->_player, this->_module_pool, this->_display_space, this->_content_pool, this->_selected_pool);

    if (space_range.has_value() && fulfilled(locked)) {
        auto const &[player, module_pool, display_space, content_pool, selected_pool] = locked;

        auto const current_frame = player->current_frame();

        if (space_range == this->_last_space_range && current_frame == this->_last_frame && !force_updating) {
            return;
        }

        auto const &space_range_value = space_range.value();
        auto const scale = display_space->scale().width;

        auto const contents = filter_map<module_content>(
            module_pool->modules(), [&space_range_value, sample_rate = this->_project_format.sample_rate, &scale,
                                     &selected_pool = selected_pool](auto const &module) {
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
