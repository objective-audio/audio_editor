//
//  ae_modules_presenter.cpp
//

#include "ae_modules_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<modules_presenter> modules_presenter::make_shared(
    std::string const &project_id, std::shared_ptr<display_space> const &display_space,
    std::shared_ptr<module_location_pool> const &location_pool) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project->editor(), display_space, location_pool);
}

std::shared_ptr<modules_presenter> modules_presenter::make_shared(
    std::shared_ptr<project_editor_for_modules_presenter> const &editor,
    std::shared_ptr<display_space> const &display_space, std::shared_ptr<module_location_pool> const &location_pool) {
    return std::shared_ptr<modules_presenter>(new modules_presenter{editor, display_space, location_pool});
}

modules_presenter::modules_presenter(std::shared_ptr<project_editor_for_modules_presenter> const &editor,
                                     std::shared_ptr<display_space> const &display_space,
                                     std::shared_ptr<module_location_pool> const &location_pool)
    : _project_editor(editor), _display_space(display_space), _location_pool(location_pool) {
    auto const sample_rate = editor->file_info().sample_rate;

    editor
        ->observe_file_track_event([this, sample_rate](file_track_event const &event) {
            switch (event.type) {
                case file_track_event_type::any:
                case file_track_event_type::reverted:
                    this->_update_all_locations(true);
                    break;
                case file_track_event_type::erased:
                    this->_location_pool->erase(event.module.value().identifier);
                    break;
                case file_track_event_type::inserted: {
                    auto const &module = event.module.value();
                    auto const space_range = this->_space_range();
                    if (space_range.has_value() && module.range.is_overlap(space_range.value())) {
                        this->_location_pool->insert(
                            module_location::make_value(module.identifier, module.range, sample_rate));
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
    return this->_location_pool->elements();
}

observing::syncable modules_presenter::observe_locations(
    std::function<void(module_location_pool_event const &)> &&handler) {
    return this->_location_pool->observe_event(std::move(handler));
}

void modules_presenter::update_if_needed() {
    this->_update_all_locations(false);
}

std::optional<proc::time::range> modules_presenter::_space_range() const {
    if (auto const editor = this->_project_editor.lock()) {
        auto const sample_rate = editor->file_info().sample_rate;
        auto const current_frame = editor->current_frame();
        return this->_display_space->frame_range(sample_rate, current_frame);
    } else {
        return std::nullopt;
    }
}

void modules_presenter::_update_all_locations(bool const force) {
    auto const space_range = this->_space_range();
    auto const editor = this->_project_editor.lock();

    if (editor && space_range.has_value()) {
        auto const current_frame = editor->current_frame();

        if (space_range == this->_last_space_range && current_frame == this->_last_frame && !force) {
            return;
        }

        auto const &space_range_value = space_range.value();

        auto const locations = filter_map<module_location>(
            editor->modules(), [&space_range_value, sample_rate = editor->file_info().sample_rate](auto const &module) {
                if (module.first.is_overlap(space_range_value)) {
                    return std::make_optional(
                        module_location::make_value(module.second.identifier, module.second.range, sample_rate));
                } else {
                    return std::optional<module_location>(std::nullopt);
                }
            });

        this->_location_pool->update_all(locations);

        this->_last_frame = current_frame;
        this->_last_space_range = space_range;
    }
}
