//
//  ae_window_presenter.cpp
//

#include "ae_window_presenter.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_project_pool.h>

using namespace yas;
using namespace yas::ae;

window_presenter::window_presenter(std::shared_ptr<project_for_window_presenter> const &project) : _project(project) {
}

std::shared_ptr<window_presenter> window_presenter::make_shared(std::string const &project_id) {
    auto const project = app_level::global()->project_pool->project_level_for_id(project_id)->project;
    return make_shared(project);
}

std::shared_ptr<window_presenter> window_presenter::make_shared(
    std::shared_ptr<project_for_window_presenter> const &project) {
    return std::shared_ptr<window_presenter>(new window_presenter{project});
}

std::string const &window_presenter::project_id() const {
    return this->_project->identifier();
}

std::string window_presenter::title() const {
    return this->_project->file_url().last_path_component();
}

bool window_presenter::should_close() {
    auto const &project = this->_project;
    if (project->can_close()) {
        project->request_close();
        return true;
    } else {
        return false;
    }
}
