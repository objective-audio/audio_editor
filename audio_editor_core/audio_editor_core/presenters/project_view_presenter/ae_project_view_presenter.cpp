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

project_view_presenter::project_view_presenter(std::shared_ptr<project> const &project)
    : _project(project), _file_track_event_fetcher(project_view_presenter_utils::make_fetcher(project)) {
    project
        ->observe_state([this, canceller = observing::cancellable_ptr{nullptr}](auto const &state) mutable {
            if (state == project_state::editing) {
                canceller =
                    this->_project->editor()
                        ->file_track()
                        ->observe_event([this](auto const &event) { this->_file_track_event_fetcher->push(event); })
                        .sync();
            }
        })
        .end()
        ->add_to(this->_pool);
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

std::string project_view_presenter::state_text() const {
    return project_view_presenter_utils::label_text(this->_project->state());
}

std::string project_view_presenter::file_info_text() const {
    return project_view_presenter_utils::label_text(this->_project->file_info());
}

std::string project_view_presenter::player_text() const {
    return project_view_presenter_utils::player_text(this->_project->player()->current_frame());
}

std::string project_view_presenter::play_button_text() const {
    return project_view_presenter_utils::play_button_text(this->_project->player()->is_playing());
}

std::string project_view_presenter::file_track_text() const {
    auto const &editor = this->_project->editor();
    return project_view_presenter_utils::file_track_text(editor ? editor->file_track()->modules() :
                                                                  file_track_module_map_t{});
}

std::string project_view_presenter::time_text() const {
    auto const &file_info = this->_project->file_info();
    auto const &player = this->_project->player();
    return project_view_presenter_utils::time_text(player->current_frame(), file_info->sample_rate);
}

void project_view_presenter::play_button_clicked() {
    this->_project->player()->toggle_playing();
}

void project_view_presenter::split_button_clicked() {
    auto const current_frame = this->_project->player()->current_frame();
    this->_project->editor()->file_track()->split_at(current_frame);
}

void project_view_presenter::drop_head_button_clicked() {
    auto const current_frame = this->_project->player()->current_frame();
    this->_project->editor()->file_track()->drop_head_and_offset_at(current_frame);
}

void project_view_presenter::drop_tail_button_clicked() {
    auto const current_frame = this->_project->player()->current_frame();
    this->_project->editor()->file_track()->drop_tail_and_offset_at(current_frame);
}

void project_view_presenter::erase_button_clicked() {
    auto const current_frame = this->_project->player()->current_frame();
    this->_project->editor()->file_track()->erase_at(current_frame);
}

void project_view_presenter::zero_button_clicked() {
    this->_project->player()->seek(0);
}

observing::syncable project_view_presenter::observe_state_text(std::function<void(std::string const &)> &&handler) {
    return this->_project->observe_state([handler = std::move(handler)](project_state const &state) {
        handler(project_view_presenter_utils::label_text(state));
    });
}

observing::syncable project_view_presenter::observe_file_info_text(std::function<void(std::string const &)> &&handler) {
    return this->_project->observe_file_info([handler = std::move(handler)](std::optional<file_info> const &file_info) {
        handler(project_view_presenter_utils::label_text(file_info));
    });
}

observing::syncable project_view_presenter::observe_play_button_text(
    std::function<void(std::string const &)> &&handler) {
    return this->_project->player()->observe_is_playing([handler = std::move(handler)](bool const is_playing) {
        handler(project_view_presenter_utils::play_button_text(is_playing));
    });
}

observing::syncable project_view_presenter::observe_file_track_text(
    std::function<void(std::string const &)> &&handler) {
    return this->_file_track_event_fetcher->observe(
        [handler = std::move(handler), this](auto const &event) { handler(this->file_track_text()); });
}
