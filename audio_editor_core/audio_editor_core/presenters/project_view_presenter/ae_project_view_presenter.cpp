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
    : _project(project),
      _file_track_event_fetcher(project_view_presenter_utils::make_file_track_fetcher(project)),
      _marker_pool_event_fetcher(project_view_presenter_utils::make_marker_pool_fetcher(project)) {
    project
        ->observe_state([this, pool = observing::canceller_pool::make_shared()](auto const &state) {
            if (state == project_state::editing) {
                auto const project = this->_project.lock();
                if (!project) {
                    return;
                }

                project->editor()
                    ->file_track()
                    ->observe_event([this](auto const &event) { this->_file_track_event_fetcher->push(event); })
                    .sync()
                    ->add_to(*pool);

                project->editor()
                    ->marker_pool()
                    ->observe_event([this](auto const &event) { this->_marker_pool_event_fetcher->push(event); })
                    .sync()
                    ->add_to(*pool);
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
    if (auto const project = this->_project.lock()) {
        return project->identifier();
    } else {
        return project_view_presenter_utils::empty_text();
    }
}

std::string project_view_presenter::state_text() const {
    if (auto const project = this->_project.lock()) {
        return project_view_presenter_utils::label_text(project->state());
    } else {
        return project_view_presenter_utils::empty_text();
    }
}

std::string project_view_presenter::file_info_text() const {
    if (auto const project = this->_project.lock()) {
        return project_view_presenter_utils::label_text(project->file_info());
    } else {
        return project_view_presenter_utils::empty_text();
    }
}

std::string project_view_presenter::player_text() const {
    if (auto const project = this->_project.lock()) {
        return project_view_presenter_utils::player_text(project->player()->current_frame(),
                                                         project->file_info()->sample_rate);
    } else {
        return project_view_presenter_utils::empty_text();
    }
}

std::string project_view_presenter::play_button_text() const {
    if (auto const project = this->_project.lock()) {
        return project_view_presenter_utils::play_button_text(project->player()->is_playing());
    } else {
        return project_view_presenter_utils::empty_text();
    }
}

std::string project_view_presenter::file_track_text() const {
    if (auto const project = this->_project.lock()) {
        auto const &editor = project->editor();
        auto const &modules = editor ? editor->file_track()->modules() : ae::empty_file_track_modules;
        return project_view_presenter_utils::file_track_text(modules);
    } else {
        return project_view_presenter_utils::empty_text();
    }
}

std::string project_view_presenter::marker_pool_text() const {
    if (auto const project = this->_project.lock()) {
        auto const &editor = project->editor();
        auto const &markers = editor ? editor->marker_pool()->markers() : ae::empty_markers;
        return project_view_presenter_utils::marker_pool_text(markers);
    } else {
        return project_view_presenter_utils::empty_text();
    }
}

std::string project_view_presenter::time_text() const {
    if (auto const project = this->_project.lock()) {
        auto const &file_info = project->file_info();
        auto const &player = project->player();
        return project_view_presenter_utils::time_text(player->current_frame(), file_info->sample_rate);
    } else {
        return project_view_presenter_utils::empty_text();
    }
}

bool project_view_presenter::is_play_button_enabled() const {
    auto const project = this->_project.lock();
    return project != nullptr;
}

bool project_view_presenter::is_split_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_split() : false;
}

bool project_view_presenter::is_drop_head_and_offset_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_split() : false;
}

bool project_view_presenter::is_drop_tail_and_offset_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_split() : false;
}

bool project_view_presenter::is_jump_previous_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_jump_to_previous_edge() : false;
}

bool project_view_presenter::is_jump_next_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_jump_to_next_edge() : false;
}

bool project_view_presenter::is_erase_and_offset_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_erase() : false;
}

bool project_view_presenter::is_insert_marker_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_insert_marker() : false;
}

bool project_view_presenter::is_zero_button_enabled() const {
    auto const project = this->_project.lock();
    return project ? project->can_return_to_zero() : false;
}

bool project_view_presenter::responds_to_action(action const action) {
    auto const project = this->_project.lock();
    if (!project) {
        return false;
    }

    switch (action) {
        case action::toggle_play:
            return true;
        case action::nudge_previous:
            return project->can_nudge();
        case action::nudge_next:
            return project->can_nudge();

        case action::jump_previous:
            return project->can_jump_to_previous_edge();
        case action::jump_next:
            return project->can_jump_to_next_edge();

        case action::drop_head_and_offset:
            return project->can_split();
        case action::split:
            return project->can_split();
        case action::drop_tail_and_offset:
            return project->can_split();

        case action::erase:
            return project->can_erase();

        case action::insert_marker:
            return project->can_insert_marker();

        case action::return_to_zero:
            return project->can_return_to_zero();
    }
}

void project_view_presenter::handle_action(action const action) {
    if (!this->responds_to_action(action)) {
        return;
    }

    auto const project = this->_project.lock();
    if (!project) {
        return;
    }

    switch (action) {
        case action::toggle_play:
            project->player()->toggle_playing();
            break;
        case action::nudge_previous:
            project->nudge_previous();
            break;
        case action::nudge_next:
            project->nudge_next();
            break;
        case action::jump_previous:
            project->jump_to_previous_edge();
            break;
        case action::jump_next:
            project->jump_to_next_edge();
            break;
        case action::drop_head_and_offset:
            project->drop_head_and_offset();
            break;
        case action::split:
            project->split();
            break;
        case action::drop_tail_and_offset:
            project->drop_tail_and_offset();
            break;
        case action::erase:
            project->erase();
            break;
        case action::insert_marker:
            project->insert_marker();
            break;
        case action::return_to_zero:
            project->return_to_zero();
            break;
    }
}

observing::syncable project_view_presenter::observe_state_text(std::function<void(std::string const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        return project->observe_state([handler = std::move(handler)](project_state const &state) {
            handler(project_view_presenter_utils::label_text(state));
        });
    } else {
        return observing::syncable{};
    }
}

observing::syncable project_view_presenter::observe_file_info_text(std::function<void(std::string const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        return project->observe_file_info([handler = std::move(handler)](std::optional<file_info> const &file_info) {
            handler(project_view_presenter_utils::label_text(file_info));
        });
    } else {
        return observing::syncable{};
    }
}

observing::syncable project_view_presenter::observe_play_button_text(
    std::function<void(std::string const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        return project->player()->observe_is_playing([handler = std::move(handler)](bool const is_playing) {
            handler(project_view_presenter_utils::play_button_text(is_playing));
        });
    } else {
        return observing::syncable{};
    }
}

observing::syncable project_view_presenter::observe_file_track_text(
    std::function<void(std::string const &)> &&handler) {
    return this->_file_track_event_fetcher->observe(
        [handler = std::move(handler), this](auto const &event) { handler(this->file_track_text()); });
}

observing::syncable project_view_presenter::observe_marker_pool_text(
    std::function<void(std::string const &)> &&handler) {
    return this->_marker_pool_event_fetcher->observe(
        [handler = std::move(handler), this](auto const &event) { handler(this->marker_pool_text()); });
}
