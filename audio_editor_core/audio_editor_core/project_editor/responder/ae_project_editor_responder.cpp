//
//  ae_project_editor_responder.cpp
//

#include "ae_project_editor_responder.h"

#include <audio_editor_core/ae_project_editor.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_responder> project_editor_responder::make_shared(
    std::shared_ptr<project_editor> const &editor) {
    return std::shared_ptr<project_editor_responder>(new project_editor_responder{editor});
}

project_editor_responder::project_editor_responder(std::shared_ptr<project_editor> const &editor) : _editor(editor) {
}

identifier project_editor_responder::responder_id() {
    return this->_responder_id;
}

std::optional<ae::action> project_editor_responder::to_action(ae::key const &key) {
    switch (key) {
        case key::space:
            return action_kind::toggle_play;
        case key::del:
            return action_kind::erase;
        case key::a:
            return action_kind::drop_head;
        case key::s:
            return action_kind::split;
        case key::d:
            return action_kind::drop_tail;
        case key::n:
            return action_kind::rotate_nudging_next_unit;
        case key::shift_n:
            return action_kind::rotate_nudging_previous_unit;
        case key::f:
            return action_kind::rotate_timing_fraction;
        case key::t:
            return action_kind::begin_time_editing;
        case key::left_bracket:
            return action_kind::set_begin_edge;
        case key::right_bracket:
            return action_kind::set_end_edge;
        case key::left:
            return action_kind::nudge_previous;
        case key::right:
            return action_kind::nudge_next;
        case key::shift_left:
            return action_kind::nudge_previous_more;
        case key::shift_right:
            return action_kind::nudge_next_more;
        case key::num_0:
            return action_kind::return_to_zero;
        case key::num_1:
            return action{action_kind::go_to_marker, "0"};
        case key::num_2:
            return action{action_kind::go_to_marker, "1"};
        case key::num_3:
            return action{action_kind::go_to_marker, "2"};
        case key::num_4:
            return action{action_kind::go_to_marker, "3"};
        case key::num_5:
            return action{action_kind::go_to_marker, "4"};
        case key::num_6:
            return action{action_kind::go_to_marker, "5"};
        case key::num_7:
            return action{action_kind::go_to_marker, "6"};
        case key::num_8:
            return action{action_kind::go_to_marker, "7"};
        case key::num_9:
            return action{action_kind::go_to_marker, "8"};

        case key::esc:
        case key::plus:
        case key::hyphen:
        case key::ret:
        case key::tab:
        case key::shift_tab:
        case key::up:
        case key::down:
            return std::nullopt;
    }
}

void project_editor_responder::handle_action(ae::action const &action) {
    auto const editor = this->_editor.lock();
    if (!editor) {
        return;
    }

    auto const responding = this->responding_to_action(action);
    switch (responding) {
        case responding::accepting: {
            switch (action.kind) {
                case action_kind::toggle_play:
                    editor->toggle_playing();
                    break;
                case action_kind::nudge_previous:
                    editor->nudge_previous(1);
                    break;
                case action_kind::nudge_next:
                    editor->nudge_next(1);
                    break;
                case action_kind::nudge_previous_more:
                    editor->nudge_previous(10);
                    break;
                case action_kind::nudge_next_more:
                    editor->nudge_next(10);
                    break;
                case action_kind::rotate_nudging_next_unit:
                    editor->rotate_nudging_next_unit();
                    break;
                case action_kind::rotate_nudging_previous_unit:
                    editor->rotate_nudging_previous_unit();
                    break;
                case action_kind::rotate_timing_fraction:
                    editor->rotate_timing_fraction();
                    break;
                case action_kind::jump_previous:
                    editor->jump_to_previous_edge();
                    break;
                case action_kind::jump_next:
                    editor->jump_to_next_edge();
                    break;
                case action_kind::jump_to_beginning:
                    editor->jump_to_beginning();
                    break;
                case action_kind::jump_to_end:
                    editor->jump_to_end();
                    break;
                case action_kind::drop_head:
                    editor->drop_head();
                    break;
                case action_kind::split:
                    editor->split();
                    break;
                case action_kind::drop_tail:
                    editor->drop_tail();
                    break;
                case action_kind::erase:
                    editor->erase();
                    break;
                case action_kind::insert_marker:
                    editor->insert_marker();
                    break;
                case action_kind::set_begin_edge:
                    editor->set_begin_edge();
                    break;
                case action_kind::set_end_edge:
                    editor->set_end_edge();
                    break;
                case action_kind::return_to_zero:
                    editor->return_to_zero();
                    break;
                case action_kind::go_to_marker:
                    editor->go_to_marker(std::stoi(action.value));
                    break;
                case action_kind::undo:
                    editor->undo();
                    break;
                case action_kind::redo:
                    editor->redo();
                    break;
                case action_kind::select_file_for_export:
                    editor->select_file_for_export();
                    break;
                case action_kind::export_to_file:
                    editor->export_to_file(url::file_url(action.value));
                    break;
                case action_kind::cut:
                    editor->cut_and_offset();
                    break;
                case action_kind::copy:
                    editor->copy();
                    break;
                case action_kind::paste:
                    editor->paste_and_offset();
                    break;

                case action_kind::begin_time_editing:
                    editor->begin_time_editing(std::nullopt);
                    break;
                case action_kind::select_time_unit:
                    editor->select_time_unit(std::stoi(action.value));
                    break;

                    // 以下、time_editor用
                case action_kind::finish_time_editing:
                case action_kind::cancel_time_editing:
                case action_kind::move_to_previous_time_unit:
                case action_kind::move_to_next_time_unit:
                case action_kind::input_time:
                case action_kind::delete_time:
                case action_kind::increment_time:
                case action_kind::decrement_time:
                case action_kind::change_time_sign_to_plus:
                case action_kind::change_time_sign_to_minus:
                    break;
            }
        } break;

        case responding::blocking:
        case responding::fallthrough:
            break;
    }
}

responding project_editor_responder::responding_to_action(ae::action const &action) {
    auto const editor = this->_editor.lock();
    if (!editor) {
        return responding::fallthrough;
    }

    static auto const to_responding = [](bool const &flag) {
        return flag ? responding::accepting : responding::blocking;
    };

    switch (action.kind) {
        case action_kind::toggle_play:
            return responding::accepting;
        case action_kind::nudge_previous:
        case action_kind::nudge_next:
        case action_kind::nudge_previous_more:
        case action_kind::nudge_next_more:
            return to_responding(editor->can_nudge());
        case action_kind::rotate_nudging_next_unit:
        case action_kind::rotate_nudging_previous_unit:
            return responding::accepting;
        case action_kind::rotate_timing_fraction:
            return responding::accepting;

        case action_kind::jump_previous:
            return to_responding(editor->can_jump_to_previous_edge());
        case action_kind::jump_next:
            return to_responding(editor->can_jump_to_next_edge());
        case action_kind::jump_to_beginning:
            return to_responding(editor->can_jump_to_beginnig());
        case action_kind::jump_to_end:
            return to_responding(editor->can_jump_to_end());

        case action_kind::drop_head:
            return to_responding(editor->can_split());
        case action_kind::split:
            return to_responding(editor->can_split());
        case action_kind::drop_tail:
            return to_responding(editor->can_split());

        case action_kind::erase:
            return to_responding(editor->can_erase());

        case action_kind::insert_marker:
            return to_responding(editor->can_insert_marker());

        case action_kind::return_to_zero:
            return to_responding(editor->can_return_to_zero());
        case action_kind::go_to_marker:
            return to_responding(editor->can_go_to_marker(std::stoi(action.value)));

        case action_kind::undo:
            return to_responding(editor->can_undo());
        case action_kind::redo:
            return to_responding(editor->can_redo());

        case action_kind::select_file_for_export:
            return to_responding(editor->can_select_file_for_export());
        case action_kind::export_to_file:
            return to_responding(editor->can_export_to_file());

        case action_kind::cut:
            return to_responding(editor->can_cut());
        case action_kind::copy:
            return to_responding(editor->can_copy());
        case action_kind::paste:
            return to_responding(editor->can_paste());

        case action_kind::cancel_time_editing:
            return to_responding(editor->can_end_time_editing());
        case action_kind::begin_time_editing:
            return to_responding(editor->can_begin_time_editing());
        case action_kind::select_time_unit:
            return to_responding(editor->can_select_time_unit());

        case action_kind::set_begin_edge:
            return to_responding(editor->can_set_begin_edge());
        case action_kind::set_end_edge:
            return to_responding(editor->can_set_end_edge());

            // 以下、time_editor用
        case action_kind::finish_time_editing:
        case action_kind::move_to_previous_time_unit:
        case action_kind::move_to_next_time_unit:
        case action_kind::input_time:
        case action_kind::delete_time:
        case action_kind::increment_time:
        case action_kind::decrement_time:
        case action_kind::change_time_sign_to_plus:
        case action_kind::change_time_sign_to_minus:
            return responding::fallthrough;
    }
}
