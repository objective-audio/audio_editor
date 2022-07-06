//
//  ae_time_editor_responder.cpp
//

#include "ae_time_editor_responder.h"

#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_closer.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_responder> time_editor_responder::make_shared(project_id const &project_id,
                                                                          time_editor *editor,
                                                                          time_editor_closer *closer) {
    return std::make_shared<time_editor_responder>(project_id, editor, closer);
}

time_editor_responder::time_editor_responder(project_id const &project_id, time_editor *editor,
                                             time_editor_closer *closer)
    : _project_id(project_id), _editor(editor), _closer(closer) {
}

std::optional<ae::action> time_editor_responder::to_action(ae::key const &key) {
    std::optional<action_id> action_id({.project_id = this->_project_id});

    switch (key) {
        case ae::key::ret:
            return action{action_kind::finish_time_editing, action_id, ""};
        case ae::key::esc:
            return action{action_kind::cancel_time_editing, action_id, ""};
        case ae::key::left:
        case ae::key::shift_tab:
            return action{action_kind::move_to_previous_time_unit, action_id, ""};
        case ae::key::right:
        case ae::key::tab:
            return action{action_kind::move_to_next_time_unit, action_id, ""};
        case ae::key::up:
            return action{action_kind::increment_time, action_id, ""};
        case ae::key::down:
            return action{action_kind::decrement_time, action_id, ""};
        case ae::key::plus:
            return action{action_kind::change_time_sign_to_plus, action_id, ""};
        case ae::key::hyphen:
            return action{action_kind::change_time_sign_to_minus, action_id, ""};

        case ae::key::num_0:
            return action{action_kind::input_time, action_id, "0"};
        case ae::key::num_1:
            return action{action_kind::input_time, action_id, "1"};
        case ae::key::num_2:
            return action{action_kind::input_time, action_id, "2"};
        case ae::key::num_3:
            return action{action_kind::input_time, action_id, "3"};
        case ae::key::num_4:
            return action{action_kind::input_time, action_id, "4"};
        case ae::key::num_5:
            return action{action_kind::input_time, action_id, "5"};
        case ae::key::num_6:
            return action{action_kind::input_time, action_id, "6"};
        case ae::key::num_7:
            return action{action_kind::input_time, action_id, "7"};
        case ae::key::num_8:
            return action{action_kind::input_time, action_id, "8"};
        case ae::key::num_9:
            return action{action_kind::input_time, action_id, "9"};
        case ae::key::del:
            return action{action_kind::delete_time, action_id, ""};

        case key::space:
        case key::a:
        case key::s:
        case key::d:
        case key::f:
        case key::n:
        case key::shift_n:
        case key::t:
        case key::left_bracket:
        case key::right_bracket:
        case key::shift_left:
        case key::shift_right:
            return std::nullopt;
    }
}

void time_editor_responder::handle_action(ae::action const &action) {
    auto const responding = this->responding_to_action(action);
    switch (responding) {
        case responding::accepting: {
            switch (action.kind) {
                case action_kind::finish_time_editing:
                    this->_closer->finish();
                    break;
                case action_kind::cancel_time_editing:
                    this->_closer->cancel();
                    break;
                case action_kind::move_to_previous_time_unit:
                    this->_editor->move_to_previous_unit();
                    break;
                case action_kind::move_to_next_time_unit:
                    this->_editor->move_to_next_unit();
                    break;
                case action_kind::input_time:
                    this->_editor->input_number(std::stoi(action.value));
                    break;
                case action_kind::delete_time:
                    this->_editor->delete_number();
                    break;
                case action_kind::increment_time:
                    this->_editor->increment_number();
                    break;
                case action_kind::decrement_time:
                    this->_editor->decrement_number();
                    break;
                case action_kind::change_time_sign_to_plus:
                    this->_editor->change_sign_to_plus();
                    break;
                case action_kind::change_time_sign_to_minus:
                    this->_editor->change_sign_to_minus();
                    break;
                case action_kind::select_time_unit:
                    this->_editor->set_unit_idx(std::stoi(action.value));
                    break;

                case action_kind::toggle_play:
                case action_kind::nudge_previous:
                case action_kind::nudge_next:
                case action_kind::nudge_previous_more:
                case action_kind::nudge_next_more:
                case action_kind::rotate_nudging_next_unit:
                case action_kind::rotate_nudging_previous_unit:
                case action_kind::rotate_timing_fraction:
                case action_kind::jump_previous:
                case action_kind::jump_next:
                case action_kind::jump_to_beginning:
                case action_kind::jump_to_end:
                case action_kind::drop_head:
                case action_kind::split:
                case action_kind::drop_tail:
                case action_kind::erase:
                case action_kind::insert_marker:
                case action_kind::set_begin_edge:
                case action_kind::set_end_edge:
                case action_kind::return_to_zero:
                case action_kind::go_to_marker:
                case action_kind::undo:
                case action_kind::redo:
                case action_kind::select_file_for_import:
                case action_kind::import_from_file:
                case action_kind::select_file_for_export:
                case action_kind::export_to_file:
                case action_kind::cut:
                case action_kind::copy:
                case action_kind::paste:
                case action_kind::purge:
                case action_kind::begin_module_renaming:
                case action_kind::begin_time_editing:
                    break;
            }
        } break;

        case responding::blocking:
        case responding::fallthrough:
            break;
    }
}

responding time_editor_responder::responding_to_action(ae::action const &action) {
    static auto const to_responding = [](bool const &flag) {
        return flag ? responding::accepting : responding::blocking;
    };

    switch (action.kind) {
        case action_kind::finish_time_editing:
            return to_responding(this->_closer->can_finish());
        case action_kind::cancel_time_editing:
            return to_responding(this->_closer->can_cancel());
        case action_kind::move_to_previous_time_unit:
            return to_responding(this->_editor->can_move_to_previous_unit());
        case action_kind::move_to_next_time_unit:
            return to_responding(this->_editor->can_move_to_next_unit());
        case action_kind::input_time:
            return to_responding(this->_editor->can_input_number());
        case action_kind::delete_time:
            return to_responding(this->_editor->can_delete_number());
        case action_kind::increment_time:
            return to_responding(this->_editor->can_increment_number());
        case action_kind::decrement_time:
            return to_responding(this->_editor->can_decrement_number());
        case action_kind::change_time_sign_to_plus:
            return to_responding(this->_editor->can_change_sign_to_plus());
        case action_kind::change_time_sign_to_minus:
            return to_responding(this->_editor->can_change_sign_to_minus());
        case action_kind::select_time_unit:
            return to_responding(this->_editor->can_set_unit_idx());

            // 以下、project_editor用
        case action_kind::toggle_play:
        case action_kind::nudge_previous:
        case action_kind::nudge_next:
        case action_kind::nudge_previous_more:
        case action_kind::nudge_next_more:
        case action_kind::rotate_nudging_next_unit:
        case action_kind::rotate_nudging_previous_unit:
        case action_kind::rotate_timing_fraction:
        case action_kind::jump_previous:
        case action_kind::jump_next:
        case action_kind::jump_to_beginning:
        case action_kind::jump_to_end:
        case action_kind::drop_head:
        case action_kind::split:
        case action_kind::drop_tail:
        case action_kind::erase:
        case action_kind::insert_marker:
        case action_kind::return_to_zero:
        case action_kind::go_to_marker:
        case action_kind::undo:
        case action_kind::redo:
        case action_kind::select_file_for_import:
        case action_kind::import_from_file:
        case action_kind::select_file_for_export:
        case action_kind::export_to_file:
        case action_kind::cut:
        case action_kind::copy:
        case action_kind::paste:
        case action_kind::purge:
        case action_kind::begin_module_renaming:
        case action_kind::begin_time_editing:
        case action_kind::set_begin_edge:
        case action_kind::set_end_edge:
            return responding::fallthrough;
    }
}