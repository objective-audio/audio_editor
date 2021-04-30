//
//  ae_project_view_presenter.cpp
//

#include "ae_project_view_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>
#include <audio_editor_core/ae_project_view_presenter_utils.h>

using namespace yas;
using namespace yas::ae;

project_view_presenter::project_view_presenter(std::shared_ptr<project> const &project) : _project(project) {
}

std::shared_ptr<project_view_presenter> project_view_presenter::make_shared(std::string const &project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project);
}

std::shared_ptr<project_view_presenter> project_view_presenter::make_shared(std::shared_ptr<project> const &project) {
    return std::shared_ptr<project_view_presenter>(new project_view_presenter{project});
}

std::string const &project_view_presenter::project_id() const {
    return this->_project->identifier();
}

std::string project_view_presenter::state_string() const {
    return project_view_presenter_utils::to_label_string(this->_project->state());
}

observing::syncable project_view_presenter::observe_state_string(std::function<void(std::string const &)> &&handler) {
    return this->_project->observe_state([handler = std::move(handler)](project_state const &state) {
        handler(project_view_presenter_utils::to_label_string(state));
    });
}
