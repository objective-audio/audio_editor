//
//  ae_modules_presenter.cpp
//

#include "ae_modules_presenter.h"

#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_track_editor.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

static std::string const empty_string = "";

std::shared_ptr<modules_presenter> modules_presenter::make_shared(window_lifetime_id const &window_lifetime_id,
                                                                  std::shared_ptr<display_space> const &display_space) {
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(window_lifetime_id);
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    return std::make_shared<modules_presenter>(project_lifetime->project_format, window_lifetime->player,
                                               project_lifetime->file_track, display_space,
                                               project_lifetime->module_location_pool);
}

modules_presenter::modules_presenter(project_format const &project_format, std::shared_ptr<player> const &player,
                                     std::shared_ptr<file_track> const &file_track,
                                     std::shared_ptr<display_space> const &display_space,
                                     std::shared_ptr<module_location_pool> const &location_pool)
    : _project_format(project_format),
      _player(player),
      _file_track(file_track),
      _display_space(display_space),
      _location_pool(location_pool) {
    auto const sample_rate = this->_project_format.sample_rate;

    file_track
        ->observe_event([this, sample_rate](file_track_event const &event) {
            auto const location_pool = this->_location_pool.lock();
            auto const display_space = this->_display_space.lock();

            if (!location_pool || !display_space) {
                return;
            }

            switch (event.type) {
                case file_track_event_type::any:
                case file_track_event_type::reverted:
                    this->_update_all_locations(true);
                    break;
                case file_track_event_type::erased:
                    location_pool->erase(event.module.value().identifier);
                    break;
                case file_track_event_type::inserted: {
                    auto const &module = event.module.value();
                    auto const space_range = this->_space_range();
                    if (space_range.has_value() && module.range.is_overlap(space_range.value())) {
                        location_pool->insert({module, sample_rate, space_range.value(), display_space->scale().width});
                    }
                } break;
                case file_track_event_type::detail_updated: {
                    auto const &module = event.module.value();
                    auto const space_range = this->_space_range();
                    if (space_range.has_value() && module.range.is_overlap(space_range.value())) {
                        location_pool->replace(
                            {module, sample_rate, space_range.value(), display_space->scale().width});
                    }
                } break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);

    display_space->observe([this](display_space_event const &event) { this->_update_all_locations(true); })
        .sync()
        ->add_to(this->_canceller_pool);
}

std::vector<std::optional<module_location>> const &modules_presenter::locations() const {
    if (auto const location_pool = this->_location_pool.lock()) {
        return location_pool->elements();
    } else {
        static std::vector<std::optional<module_location>> const _empty;
        return _empty;
    }
}

observing::syncable modules_presenter::observe_locations(
    std::function<void(module_location_pool_event const &)> &&handler) {
    if (auto const location_pool = this->_location_pool.lock()) {
        return location_pool->observe_event(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

std::string const &modules_presenter::name_for_range(time::range const &range) {
    if (auto const file_track = this->_file_track.lock()) {
        if (file_track->modules().contains(range)) {
            return file_track->modules().at(range).name;
        }
    }

    return empty_string;
}

void modules_presenter::update_if_needed() {
    this->_update_all_locations(false);
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

void modules_presenter::_update_all_locations(bool const force) {
    auto const space_range = this->_space_range();
    auto const player = this->_player.lock();
    auto const file_track = this->_file_track.lock();
    auto const display_space = this->_display_space.lock();
    auto const location_pool = this->_location_pool.lock();

    if (player && file_track && space_range.has_value() && display_space && location_pool) {
        auto const current_frame = player->current_frame();

        if (space_range == this->_last_space_range && current_frame == this->_last_frame && !force) {
            return;
        }

        auto const &space_range_value = space_range.value();
        auto const scale = display_space->scale().width;

        auto const locations = filter_map<module_location>(
            file_track->modules(),
            [&space_range_value, sample_rate = this->_project_format.sample_rate, &scale](auto const &module) {
                if (module.first.is_overlap(space_range_value)) {
                    return std::make_optional<module_location>(module.second, sample_rate, space_range_value, scale);
                } else {
                    return std::optional<module_location>(std::nullopt);
                }
            });

        location_pool->update_all(locations, false);

        this->_last_frame = current_frame;
        this->_last_space_range = space_range;
    }
}
