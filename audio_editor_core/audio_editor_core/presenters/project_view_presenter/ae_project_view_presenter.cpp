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

std::string project_view_presenter::file_info_string() const {
    return project_view_presenter_utils::to_label_string(this->_project->file_info());
}

std::string project_view_presenter::player_string() const {
    return project_view_presenter_utils::to_player_string(this->_project->player()->current_frame());
}

std::string project_view_presenter::play_button_string() const {
    return project_view_presenter_utils::to_play_button_string(this->_project->player()->is_playing());
}

void project_view_presenter::play_button_clicked() {
    this->_project->player()->toggle_playing();
}

observing::syncable project_view_presenter::observe_state_string(std::function<void(std::string const &)> &&handler) {
    return this->_project->observe_state([handler = std::move(handler)](project_state const &state) {
        handler(project_view_presenter_utils::to_label_string(state));
    });
}

observing::syncable project_view_presenter::observe_file_info_string(
    std::function<void(std::string const &)> &&handler) {
    return this->_project->observe_file_info([handler = std::move(handler)](std::optional<file_info> const &file_info) {
        handler(project_view_presenter_utils::to_label_string(file_info));
    });
}

observing::syncable project_view_presenter::observe_play_button_string(
    std::function<void(std::string const &)> &&handler) {
    return this->_project->player()->observe_is_playing([handler = std::move(handler)](bool const is_playing) {
        handler(project_view_presenter_utils::to_play_button_string(is_playing));
    });
}
