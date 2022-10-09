//
//  ae_pasting_modules_presenter.cpp
//

#include "ae_pasting_modules_presenter.hpp"

#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_pasteboard.h>

using namespace yas;
using namespace yas::ae;

static std::vector<std::optional<pasting_module_location>> const _empty_locations;

std::shared_ptr<pasting_modules_presenter> pasting_modules_presenter::make_shared(
    window_lifetime_id const &window_lifetime_id, std::shared_ptr<display_space> const &display_space) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    return std::make_shared<pasting_modules_presenter>(project_lifetime->project_format, project_lifetime->pasteboard,
                                                       display_space, project_lifetime->pasting_module_location_pool);
}

pasting_modules_presenter::pasting_modules_presenter(project_format const &project_format,
                                                     std::shared_ptr<pasteboard> const &pasteboard,
                                                     std::shared_ptr<display_space> const &display_space,
                                                     std::shared_ptr<pasting_module_location_pool> const &location_pool)
    : _project_format(project_format),
      _pasteboard(pasteboard),
      _display_space(display_space),
      _location_pool(location_pool) {
    pasteboard
        ->observe_event([this](pasteboard_event const &event) {
            switch (event) {
                case pasteboard_event::fetched:
                case pasteboard_event::file_modules:
                case pasteboard_event::cleared:
                    this->_update_all_locations(true, true);
                    break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);

    // pasteboardのfetchedでupdateしているのでendにして最初はスルー
    display_space->observe([this](display_space_event const &event) { this->_update_all_locations(true, false); })
        .end()
        ->add_to(this->_canceller_pool);
}

std::vector<std::optional<pasting_module_location>> const &pasting_modules_presenter::locations() const {
    if (auto const location_pool = this->_location_pool.lock()) {
        return location_pool->elements();
    } else {
        return _empty_locations;
    }
}

observing::syncable pasting_modules_presenter::observe_locations(
    std::function<void(pasting_module_location_pool_event const &)> &&handler) {
    if (auto const location_pool = this->_location_pool.lock()) {
        return location_pool->observe_event(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

void pasting_modules_presenter::update_if_needed() {
    this->_update_all_locations(false, false);
}

std::optional<time::range> pasting_modules_presenter::_space_range() const {
    if (auto const display_space = this->_display_space.lock()) {
        auto const sample_rate = this->_project_format.sample_rate;
        return display_space->frame_range(sample_rate, 0);
    } else {
        return std::nullopt;
    }
}

void pasting_modules_presenter::_update_all_locations(bool const force_updating, bool const force_replacing) {
    auto const space_range = this->_space_range();
    auto const pasteboard = this->_pasteboard.lock();
    auto const display_space = this->_display_space.lock();
    auto const location_pool = this->_location_pool.lock();

    if (pasteboard && space_range.has_value() && display_space && location_pool) {
        if (space_range == this->_last_space_range && !force_updating) {
            return;
        }

        auto const &space_range_value = space_range.value();
        auto const scale = display_space->scale().width;

        auto const locations = filter_map<pasting_module_location>(
            pasteboard->file_modules(), [&space_range_value, sample_rate = this->_project_format.sample_rate,
                                         &scale](pasting_file_module_object const &module) {
                if (module.value.range.is_overlap(space_range_value)) {
                    return std::make_optional<pasting_module_location>(module, sample_rate, scale);
                } else {
                    return std::optional<pasting_module_location>(std::nullopt);
                }
            });

        location_pool->update_all(locations, force_replacing);

        this->_last_space_range = space_range;
    }
}
