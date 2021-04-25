//
//  ae_window_presenter.cpp
//

#include "ae_window_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>

using namespace yas;
using namespace yas::ae;

window_presenter::window_presenter(std::shared_ptr<project> const &project) : _project(project) {
}

std::shared_ptr<window_presenter> window_presenter::make_shared(std::string const &project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project);
}

std::shared_ptr<window_presenter> window_presenter::make_shared(std::shared_ptr<project> const &project) {
    return std::shared_ptr<window_presenter>(new window_presenter{project});
}

std::string const &window_presenter::project_id() const {
    return this->_project->identifier();
}

std::string window_presenter::title() const {
    return this->_project->file_url().last_path_component();
}

bool window_presenter::should_close() {
    auto const project = this->_project;
    bool const can_close = project->can_close();
    project->request_close();
    return can_close;
}
