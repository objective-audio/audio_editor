//
//  ae_window_presenter.cpp
//

#include "ae_window_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project.h>

using namespace yas;
using namespace yas::ae;

window_presenter::window_presenter(std::string const &project_id, url const &file_url,
                                   std::shared_ptr<project_for_window_presenter> const &project)
    : project_id(project_id), _file_url(file_url), _project(project) {
}

std::shared_ptr<window_presenter> window_presenter::make_shared(std::string const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return make_shared(project_id, project_level->file_url, project_level->project);
}

std::shared_ptr<window_presenter> window_presenter::make_shared(
    std::string const &project_id, url const &file_url, std::shared_ptr<project_for_window_presenter> const &project) {
    return std::shared_ptr<window_presenter>(new window_presenter{project_id, file_url, project});
}

std::string window_presenter::title() const {
    return this->_file_url.last_path_component();
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
