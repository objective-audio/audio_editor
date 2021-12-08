//
//  ae_modules_presenter.cpp
//

#include "ae_modules_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<modules_presenter> modules_presenter::make_shared(std::string const &project_id,
                                                                  std::shared_ptr<display_space> const &display_space) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project->editor(), display_space);
}

std::shared_ptr<modules_presenter> modules_presenter::make_shared(
    std::shared_ptr<project_editor_for_modules_presenter> const &editor,
    std::shared_ptr<display_space> const &display_space) {
    return std::shared_ptr<modules_presenter>(new modules_presenter{editor, display_space});
}

modules_presenter::modules_presenter(std::shared_ptr<project_editor_for_modules_presenter> const &editor,
                                     std::shared_ptr<display_space> const &display_space)
    : _project_editor(editor), _display_space(display_space), _location_pool(module_location_pool::make_shared()) {
    editor
        ->observe_file_track_event([this,
                                    sample_rate = editor->file_info().sample_rate](file_track_event const &event) {
            switch (event.type) {
                case file_track_event_type::any:
                case file_track_event_type::reverted: {
                    auto const locations = to_vector<module_location>(event.modules, [&sample_rate](auto const &pair) {
                        return module_location::make(pair.second.identifier, pair.second.range, sample_rate);
                    });
                    this->_location_pool->update_all(locations);
                } break;
                case file_track_event_type::erased:
                    this->_location_pool->erase(event.module.value().identifier);
                    break;
                case file_track_event_type::inserted:
                    auto const &module = event.module.value();
                    this->_location_pool->insert(module_location::make(module.identifier, module.range, sample_rate));
                    break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);
}

std::vector<std::optional<module_location>> const &modules_presenter::locations() const {
    return this->_location_pool->locations();
}

observing::syncable modules_presenter::observe_locations(
    std::function<void(module_location_pool_event const &)> &&handler) {
    return this->_location_pool->observe_event(std::move(handler));
}
