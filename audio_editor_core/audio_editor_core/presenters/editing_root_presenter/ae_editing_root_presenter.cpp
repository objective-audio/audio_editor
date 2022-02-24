//
//  ae_editing_root_presenter.cpp
//

#include "ae_editing_root_presenter.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_editing_root_presenter_utils.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_project_pool.h>

using namespace yas;
using namespace yas::ae;

editing_root_presenter::editing_root_presenter(
    std::shared_ptr<project_for_editing_root_presenter> const &project,
    std::shared_ptr<project_editor_for_editing_root_presenter> const &editor,
    std::shared_ptr<action_router_for_editing_root_presenter> const &action_router)
    : _project(project),
      _project_editor(editor),
      _action_router(action_router),
      _file_track_event_fetcher(editing_root_presenter_utils::make_file_track_fetcher(editor)),
      _marker_pool_event_fetcher(editing_root_presenter_utils::make_marker_pool_fetcher(editor)) {
    editor->observe_file_track_event([this](auto const &event) { this->_file_track_event_fetcher->push(event); })
        .sync()
        ->add_to(this->_pool);

    editor->observe_marker_pool_event([this](auto const &event) { this->_marker_pool_event_fetcher->push(event); })
        .end()
        ->add_to(this->_pool);
}

std::shared_ptr<editing_root_presenter> editing_root_presenter::make_shared(std::string const &project_id) {
    auto const project = app_level::global()->project_pool->project_level_for_id(project_id)->project;
    return make_shared(project, project->editor, project->action_router);
}

std::shared_ptr<editing_root_presenter> editing_root_presenter::make_shared(
    std::shared_ptr<project_for_editing_root_presenter> const &project,
    std::shared_ptr<project_editor_for_editing_root_presenter> const &editor,
    std::shared_ptr<action_router_for_editing_root_presenter> const &action_router) {
    return std::shared_ptr<editing_root_presenter>(new editing_root_presenter{project, editor, action_router});
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
    if (auto const editor = this->_project_editor.lock()) {
        return editing_root_presenter_utils::label_text(editor->file_info());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::file_track_text() const {
    if (auto const editor = this->_project_editor.lock()) {
        return editing_root_presenter_utils::file_track_text(editor->modules());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::marker_pool_text() const {
    if (auto const editor = this->_project_editor.lock()) {
        return editing_root_presenter_utils::marker_pool_text(editor->markers());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

playing_line_state_t editing_root_presenter::playing_line_state() const {
    if (auto const editor = this->_project_editor.lock()) {
        if (editor->is_scrolling()) {
            return playing_line_state_t::scrolling;
        } else if (editor->is_playing()) {
            return playing_line_state_t::playing;
        }
    }

    return playing_line_state_t::pausing;
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

bool editing_root_presenter::responds_to_action(action const action) {
    auto const project = this->_project.lock();
    auto const editor = this->_project_editor.lock();
    auto const router = this->_action_router.lock();
    if (!project || !editor || !router) {
        return false;
    }

    if (!router->responds_to_action(action.kind)) {
        return false;
    }

    switch (action.kind) {
        case action_kind::toggle_play:
            return true;
        case action_kind::nudge_previous:
        case action_kind::nudge_next:
        case action_kind::nudge_previous_more:
        case action_kind::nudge_next_more:
            return editor->can_nudge();
        case action_kind::rotate_nudging_next_unit:
        case action_kind::rotate_nudging_previous_unit:
            return true;
        case action_kind::rotate_timing_fraction:
            return true;

        case action_kind::jump_previous:
            return editor->can_jump_to_previous_edge();
        case action_kind::jump_next:
            return editor->can_jump_to_next_edge();
        case action_kind::jump_to_beginning:
            return editor->can_jump_to_beginnig();
        case action_kind::jump_to_end:
            return editor->can_jump_to_end();

        case action_kind::drop_head:
            return editor->can_split();
        case action_kind::split:
            return editor->can_split();
        case action_kind::drop_tail:
            return editor->can_split();

        case action_kind::erase:
            return editor->can_erase();

        case action_kind::insert_marker:
            return editor->can_insert_marker();

        case action_kind::return_to_zero:
            return editor->can_return_to_zero();
        case action_kind::go_to_marker:
            return editor->can_go_to_marker(std::stoi(action.value));

        case action_kind::undo:
            return editor->can_undo();
        case action_kind::redo:
            return editor->can_redo();

        case action_kind::select_file_for_export:
            return editor->can_select_file_for_export();

        case action_kind::cut:
            return editor->can_cut();
        case action_kind::copy:
            return editor->can_copy();
        case action_kind::paste:
            return editor->can_paste();

        case action_kind::cancel_time_editing:
            return editor->can_end_time_editing();
        case action_kind::begin_time_editing:
            return editor->can_begin_time_editing();
        case action_kind::finish_time_editing:
        case action_kind::move_to_previous_time_unit:
        case action_kind::move_to_next_time_unit:
        case action_kind::input_time:
        case action_kind::delete_time:
        case action_kind::increment_time:
        case action_kind::decrement_time:
        case action_kind::change_time_sign_to_plus:
        case action_kind::change_time_sign_to_minus:
        case action_kind::select_time_unit:
        case action_kind::set_begin_edge:
        case action_kind::set_end_edge:
            return false;
    }
}
