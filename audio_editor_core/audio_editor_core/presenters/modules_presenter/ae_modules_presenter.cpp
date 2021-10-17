//
//  ae_modules_presenter.cpp
//

#include "ae_modules_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<modules_presenter> modules_presenter::make_shared(std::string const &project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project);
}

std::shared_ptr<modules_presenter> modules_presenter::make_shared(std::shared_ptr<project> const &project) {
    return std::shared_ptr<modules_presenter>(new modules_presenter{project});
}

modules_presenter::modules_presenter(std::shared_ptr<project> const &project) : _project(project) {
}

std::vector<module_location> modules_presenter::module_locations() const {
    if (auto const project = this->_project.lock()) {
        auto const &editor = project->editor();
        auto const &file_info = project->file_info();
        if (editor && file_info) {
            auto const &modules = editor->file_track()->modules();
            auto const &sample_rate = file_info.value().sample_rate;

            return to_vector<module_location>(modules, [&sample_rate](auto const &pair) {
                float const x = static_cast<double>(pair.second.range.frame) / static_cast<double>(sample_rate);
                float const width = static_cast<double>(pair.second.range.length) / static_cast<double>(sample_rate);
                return module_location{x, width};
            });
        }
    }

    return {};
}

observing::syncable modules_presenter::observe_modules(
    std::function<void(std::vector<module_location> const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        if (auto const &editor = project->editor()) {
            return editor->file_track()->observe_event(
                [this, handler = std::move(handler)](auto const &) { handler(this->module_locations()); });
        }
    }
    return observing::syncable{};
}
