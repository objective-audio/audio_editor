//
//  ae_editing_root_presenter.cpp
//

#include "ae_editing_root_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_editing_root_presenter_utils.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_editor.h>
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
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project, project->editor(), project->action_router());
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

std::string editing_root_presenter::play_button_text() const {
    if (auto const editor = this->_project_editor.lock()) {
        return editing_root_presenter_utils::play_button_text(editor->is_playing());
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

std::string editing_root_presenter::nudge_text() const {
    if (auto const editor = this->_project_editor.lock()) {
        return editing_root_presenter_utils::nudge_text(editor->nudging_kind());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::timing_text() const {
    if (auto const editor = this->_project_editor.lock()) {
        return editing_root_presenter_utils::timing_text(editor->timing_fraction_kind());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

bool editing_root_presenter::is_play_button_enabled() const {
    auto const project = this->_project.lock();
    return project != nullptr;
}

bool editing_root_presenter::is_split_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_split() : false;
}

bool editing_root_presenter::is_drop_head_and_offset_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_split() : false;
}

bool editing_root_presenter::is_drop_tail_and_offset_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_split() : false;
}

bool editing_root_presenter::is_jump_previous_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_jump_to_previous_edge() : false;
}

bool editing_root_presenter::is_jump_next_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_jump_to_next_edge() : false;
}

bool editing_root_presenter::is_erase_and_offset_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_erase() : false;
}

bool editing_root_presenter::is_insert_marker_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_insert_marker() : false;
}

bool editing_root_presenter::is_zero_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_return_to_zero() : false;
}

bool editing_root_presenter::is_undo_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_undo() : false;
}

bool editing_root_presenter::is_redo_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_redo() : false;
}

bool editing_root_presenter::is_export_button_enabled() const {
    auto const editor = this->_project_editor.lock();
    return editor ? editor->can_select_file_for_export() : false;
}

bool editing_root_presenter::is_nudge_button_enabled() const {
    return true;
}

bool editing_root_presenter::is_timing_button_enabled() const {
    return true;
}

playing_line_state_t editing_root_presenter::playing_line_state() const {
    if (auto const editor = this->_project_editor.lock()) {
        if (editor->is_scrolling()) {
            return playing_line_state_t::scrolling;
        } else if (editor->is_playing()) {
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

observing::syncable editing_root_presenter::observe_play_button_text(
    std::function<void(std::string const &)> &&handler) {
    if (auto const editor = this->_project_editor.lock()) {
        return editor->observe_is_playing([handler = std::move(handler)](bool const is_playing) {
            handler(editing_root_presenter_utils::play_button_text(is_playing));
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

observing::syncable editing_root_presenter::observe_nudging_text(std::function<void(std::string const &)> &&handler) {
    if (auto const editor = this->_project_editor.lock()) {
        return editor->observe_nudging_kind([handler = std::move(handler)](ae::nudging_kind const &kind) {
            handler(editing_root_presenter_utils::nudge_text(kind));
        });
    } else {
        return observing::syncable{};
    }
}

observing::syncable editing_root_presenter::observe_timing_text(std::function<void(std::string const &)> &&handler) {
    if (auto const editor = this->_project_editor.lock()) {
        return editor->observe_timing_fraction([handler = std::move(handler)](ae::timing_fraction_kind const &kind) {
            handler(editing_root_presenter_utils::timing_text(kind));
        });
        return observing::syncable{};
    } else {
        return observing::syncable{};
    }
}

bool editing_root_presenter::responds_to_action(action const action) {
    auto const project = this->_project.lock();
    auto const editor = this->_project_editor.lock();
    auto const router = this->_action_router.lock();
    if (!project || !editor || !router) {
        return false;
    }

    if (!router->responds_to_action(action)) {
        return false;
    }

    switch (action) {
        case action::toggle_play:
            return true;
        case action::nudge_previous:
            return editor->can_nudge();
        case action::nudge_next:
            return editor->can_nudge();
        case action::rotate_nudging_kind:
            return true;
        case action::rotate_timing_fraction:
            return true;

        case action::jump_previous:
            return editor->can_jump_to_previous_edge();
        case action::jump_next:
            return editor->can_jump_to_next_edge();
        case action::jump_to_beginning:
            return editor->can_jump_to_beginnig();
        case action::jump_to_end:
            return editor->can_jump_to_end();

        case action::drop_head_and_offset:
            return editor->can_split();
        case action::split:
            return editor->can_split();
        case action::drop_tail_and_offset:
            return editor->can_split();

        case action::erase_and_offset:
            return editor->can_erase();

        case action::insert_marker:
            return editor->can_insert_marker();

        case action::return_to_zero:
            return editor->can_return_to_zero();
        case action::go_to_marker_1:
            return editor->can_go_to_marker(0);
        case action::go_to_marker_2:
            return editor->can_go_to_marker(1);
        case action::go_to_marker_3:
            return editor->can_go_to_marker(2);
        case action::go_to_marker_4:
            return editor->can_go_to_marker(3);
        case action::go_to_marker_5:
            return editor->can_go_to_marker(4);
        case action::go_to_marker_6:
            return editor->can_go_to_marker(5);
        case action::go_to_marker_7:
            return editor->can_go_to_marker(6);
        case action::go_to_marker_8:
            return editor->can_go_to_marker(7);
        case action::go_to_marker_9:
            return editor->can_go_to_marker(8);

        case action::undo:
            return editor->can_undo();
        case action::redo:
            return editor->can_redo();

        case action::select_file_for_export:
            return editor->can_select_file_for_export();

        case action::cut:
            return editor->can_cut();
        case action::copy:
            return editor->can_copy();
        case action::paste:
            return editor->can_paste();

        case action::cancel_time_editing:
            return editor->can_end_time_editing();
        case action::begin_time_editing:
            return editor->can_begin_time_editing();
        case action::finish_time_editing:
        case action::move_to_previous_time_unit:
        case action::move_to_next_time_unit:
        case action::input_time_0:
        case action::input_time_1:
        case action::input_time_2:
        case action::input_time_3:
        case action::input_time_4:
        case action::input_time_5:
        case action::input_time_6:
        case action::input_time_7:
        case action::input_time_8:
        case action::input_time_9:
        case action::delete_time:
        case action::change_time_sign_to_plus:
        case action::change_time_sign_to_minus:
            return false;
    }
}
