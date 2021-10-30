//
//  ae_editing_root_presenter.cpp
//

#include "ae_editing_root_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_editing_root_presenter_utils.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>

using namespace yas;
using namespace yas::ae;

editing_root_presenter::editing_root_presenter(std::shared_ptr<project> const &project)
    : _project(project),
      _file_track_event_fetcher(editing_root_presenter_utils::make_file_track_fetcher(project)),
      _marker_pool_event_fetcher(editing_root_presenter_utils::make_marker_pool_fetcher(project)) {
    project->editor()
        ->file_track()
        ->observe_event([this](auto const &event) { this->_file_track_event_fetcher->push(event); })
        .sync()
        ->add_to(this->_pool);

    project->editor()
        ->marker_pool()
        ->observe_event([this](auto const &event) { this->_marker_pool_event_fetcher->push(event); })
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<editing_root_presenter> editing_root_presenter::make_shared(std::string const &project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project);
}

std::shared_ptr<editing_root_presenter> editing_root_presenter::make_shared(std::shared_ptr<project> const &project) {
    return std::shared_ptr<editing_root_presenter>(new editing_root_presenter{project});
}

std::string const &editing_root_presenter::project_id() const {
    if (auto const project = this->_project.lock()) {
        return project->identifier();
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::state_text() const {
    if (auto const project = this->_project.lock()) {
        return editing_root_presenter_utils::label_text(project->state());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::file_info_text() const {
    if (auto const project = this->_project.lock()) {
        return editing_root_presenter_utils::label_text(project->file_info());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::player_text() const {
    if (auto const project = this->_project.lock()) {
        return editing_root_presenter_utils::player_text(project->player()->current_frame(),
                                                         project->file_info()->sample_rate);
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::play_button_text() const {
    if (auto const project = this->_project.lock()) {
        return editing_root_presenter_utils::play_button_text(project->player()->is_playing());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::file_track_text() const {
    if (auto const project = this->_project.lock()) {
        auto const &editor = project->editor();
        auto const &modules = editor ? editor->file_track()->modules() : ae::empty_file_track_modules;
        return editing_root_presenter_utils::file_track_text(modules);
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::marker_pool_text() const {
    if (auto const project = this->_project.lock()) {
        auto const &editor = project->editor();
        auto const &markers = editor ? editor->marker_pool()->markers() : ae::empty_markers;
        return editing_root_presenter_utils::marker_pool_text(markers);
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::time_text() const {
    if (auto const project = this->_project.lock()) {
        auto const &file_info = project->file_info();
        auto const &player = project->player();
        return editing_root_presenter_utils::time_text(player->current_frame(), file_info->sample_rate);
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

bool editing_root_presenter::is_play_button_enabled() const {
    auto const project = this->_project.lock();
    return project != nullptr;
}

bool editing_root_presenter::is_split_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_split() : false;
}

bool editing_root_presenter::is_drop_head_and_offset_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_split() : false;
}

bool editing_root_presenter::is_drop_tail_and_offset_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_split() : false;
}

bool editing_root_presenter::is_jump_previous_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_jump_to_previous_edge() : false;
}

bool editing_root_presenter::is_jump_next_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_jump_to_next_edge() : false;
}

bool editing_root_presenter::is_erase_and_offset_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_erase() : false;
}

bool editing_root_presenter::is_insert_marker_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_insert_marker() : false;
}

bool editing_root_presenter::is_zero_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_return_to_zero() : false;
}

playing_line_state_t editing_root_presenter::playing_line_state() const {
    if (auto const project = this->_project.lock()) {
        if (project->player()->is_playing()) {
            return playing_line_state_t::playing;
        }
    }

    return playing_line_state_t::nothing;
}

observing::syncable editing_root_presenter::observe_state_text(std::function<void(std::string const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        return project->observe_state([handler = std::move(handler)](project_state const &state) {
            handler(editing_root_presenter_utils::label_text(state));
        });
    } else {
        return observing::syncable{};
    }
}

observing::syncable editing_root_presenter::observe_file_info_text(std::function<void(std::string const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        return project->observe_file_info([handler = std::move(handler)](std::optional<file_info> const &file_info) {
            handler(editing_root_presenter_utils::label_text(file_info));
        });
    } else {
        return observing::syncable{};
    }
}

observing::syncable editing_root_presenter::observe_play_button_text(
    std::function<void(std::string const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        return project->player()->observe_is_playing([handler = std::move(handler)](bool const is_playing) {
            handler(editing_root_presenter_utils::play_button_text(is_playing));
        });
    } else {
        return observing::syncable{};
    }
}

observing::syncable editing_root_presenter::observe_playing_line_state(
    std::function<void(playing_line_state_t const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        return project->player()->observe_is_playing(
            [handler = std::move(handler), this](bool const is_playing) { handler(this->playing_line_state()); });
    } else {
        return observing::syncable{};
    }
}

observing::syncable editing_root_presenter::observe_file_track_text(
    std::function<void(std::string const &)> &&handler) {
    return this->_file_track_event_fetcher->observe(
        [handler = std::move(handler), this](auto const &event) { handler(this->file_track_text()); });
}

observing::syncable editing_root_presenter::observe_marker_pool_text(
    std::function<void(std::string const &)> &&handler) {
    return this->_marker_pool_event_fetcher->observe(
        [handler = std::move(handler), this](auto const &event) { handler(this->marker_pool_text()); });
}
