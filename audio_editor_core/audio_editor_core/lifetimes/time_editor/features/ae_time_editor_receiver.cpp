//
//  ae_time_editor_receiver.cpp
//

#include "ae_time_editor_receiver.h"

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_closer.h>

using namespace yas;
using namespace yas::ae;

time_editor_receiver::time_editor_receiver(project_sub_lifetime_id const &lifetime_id,
                                           std::shared_ptr<database> const &database, time_editor *editor,
                                           time_editor_closer *closer)
    : _lifetime_id(lifetime_id), _database(database), _editor(editor), _closer(closer) {
}

std::optional<action_id> time_editor_receiver::receivable_id() const {
    return action_id{this->_lifetime_id.window};
}

std::optional<ae::action> time_editor_receiver::to_action(ae::key const &key) const {
    switch (key) {
        case ae::key::ret:
            return action{time_editing_action_name::finish_time_editing, ""};
        case ae::key::esc:
            return action{time_editing_action_name::cancel_time_editing, ""};
        case ae::key::left:
        case ae::key::shift_tab:
            return action{time_editing_action_name::move_to_previous_time_unit, ""};
        case ae::key::right:
        case ae::key::tab:
            return action{time_editing_action_name::move_to_next_time_unit, ""};
        case ae::key::up:
            return action{time_editing_action_name::increment_time, ""};
        case ae::key::down:
            return action{time_editing_action_name::decrement_time, ""};
        case ae::key::plus:
            return action{time_editing_action_name::change_time_sign_to_plus, ""};
        case ae::key::hyphen:
            return action{time_editing_action_name::change_time_sign_to_minus, ""};

        case ae::key::num_0:
            return action{time_editing_action_name::input_time, "0"};
        case ae::key::num_1:
            return action{time_editing_action_name::input_time, "1"};
        case ae::key::num_2:
            return action{time_editing_action_name::input_time, "2"};
        case ae::key::num_3:
            return action{time_editing_action_name::input_time, "3"};
        case ae::key::num_4:
            return action{time_editing_action_name::input_time, "4"};
        case ae::key::num_5:
            return action{time_editing_action_name::input_time, "5"};
        case ae::key::num_6:
            return action{time_editing_action_name::input_time, "6"};
        case ae::key::num_7:
            return action{time_editing_action_name::input_time, "7"};
        case ae::key::num_8:
            return action{time_editing_action_name::input_time, "8"};
        case ae::key::num_9:
            return action{time_editing_action_name::input_time, "9"};
        case ae::key::del:
            return action{time_editing_action_name::delete_time, ""};

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

void time_editor_receiver::receive(ae::action const &action) const {
    switch (this->receivable_state(action)) {
        case action_receivable_state::accepting: {
            switch (to_kind(action.name)) {
                case action_name_kind::time_editing: {
                    if (auto const database = this->_database.lock()) {
                        database->suspend_saving([this, &action] {
                            switch (get<time_editing_action_name>(action.name)) {
                                case time_editing_action_name::finish_time_editing:
                                    this->_closer->finish();
                                    break;
                                case time_editing_action_name::cancel_time_editing:
                                    this->_closer->cancel();
                                    break;
                                case time_editing_action_name::move_to_previous_time_unit:
                                    this->_editor->move_to_previous_unit();
                                    break;
                                case time_editing_action_name::move_to_next_time_unit:
                                    this->_editor->move_to_next_unit();
                                    break;
                                case time_editing_action_name::input_time:
                                    this->_editor->input_number(static_cast<uint32_t>(action.integer_value()));
                                    break;
                                case time_editing_action_name::delete_time:
                                    this->_editor->delete_number();
                                    break;
                                case time_editing_action_name::increment_time:
                                    this->_editor->increment_number();
                                    break;
                                case time_editing_action_name::decrement_time:
                                    this->_editor->decrement_number();
                                    break;
                                case time_editing_action_name::change_time_sign_to_plus:
                                    this->_editor->change_sign_to_plus();
                                    break;
                                case time_editing_action_name::change_time_sign_to_minus:
                                    this->_editor->change_sign_to_minus();
                                    break;
                                case time_editing_action_name::select_time_unit:
                                    this->_editor->set_unit_idx(action.integer_value());
                                    break;
                            }
                        });
                    }
                } break;
                case action_name_kind::editing:
                    break;
            }
        } break;

        case action_receivable_state::blocking:
        case action_receivable_state::fallthrough:
            break;
    }
}

action_receivable_state time_editor_receiver::receivable_state(ae::action const &action) const {
    switch (to_kind(action.name)) {
        case action_name_kind::time_editing: {
            static auto const to_state = [](bool const &flag) {
                return flag ? action_receivable_state::accepting : action_receivable_state::blocking;
            };

            switch (get<time_editing_action_name>(action.name)) {
                case time_editing_action_name::finish_time_editing:
                    return to_state(this->_closer->can_finish());
                case time_editing_action_name::cancel_time_editing:
                    return to_state(this->_closer->can_cancel());
                case time_editing_action_name::move_to_previous_time_unit:
                    return to_state(this->_editor->can_move_to_previous_unit());
                case time_editing_action_name::move_to_next_time_unit:
                    return to_state(this->_editor->can_move_to_next_unit());
                case time_editing_action_name::input_time:
                    return to_state(this->_editor->can_input_number());
                case time_editing_action_name::delete_time:
                    return to_state(this->_editor->can_delete_number());
                case time_editing_action_name::increment_time:
                    return to_state(this->_editor->can_increment_number());
                case time_editing_action_name::decrement_time:
                    return to_state(this->_editor->can_decrement_number());
                case time_editing_action_name::change_time_sign_to_plus:
                    return to_state(this->_editor->can_change_sign_to_plus());
                case time_editing_action_name::change_time_sign_to_minus:
                    return to_state(this->_editor->can_change_sign_to_minus());
                case time_editing_action_name::select_time_unit:
                    return to_state(this->_editor->can_set_unit_idx());
            }
        }

        case action_name_kind::editing:
            return action_receivable_state::fallthrough;
    }
}
