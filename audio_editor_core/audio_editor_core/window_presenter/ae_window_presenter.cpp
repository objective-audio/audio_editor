//
//  ae_window_presenter.cpp
//

#include "ae_window_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>

using namespace yas;
using namespace yas::ae;

window_presenter::window_presenter(project_ptr const &project) : _project(project) {
}

std::shared_ptr<window_presenter> window_presenter::make_shared(uintptr_t const project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project);
}

std::shared_ptr<window_presenter> window_presenter::make_shared(project_ptr const &project) {
    return std::shared_ptr<window_presenter>(new window_presenter{project});
}

std::string window_presenter::title() const {
    return this->_project->file_url().last_path_component();
}
