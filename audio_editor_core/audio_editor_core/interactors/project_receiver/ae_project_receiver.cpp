//
//  ae_project_receiver.cpp
//

#include "ae_project_receiver.h"

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_edge_editor.h>
#include <audio_editor_core/ae_export_interactor.h>
#include <audio_editor_core/ae_import_interactor.h>
#include <audio_editor_core/ae_jumper.h>
#include <audio_editor_core/ae_marker_editor.h>
#include <audio_editor_core/ae_module_renaming_launcher.h>
#include <audio_editor_core/ae_nudge_settings.h>
#include <audio_editor_core/ae_nudger.h>
#include <audio_editor_core/ae_playing_toggler.h>
#include <audio_editor_core/ae_reverter.h>
#include <audio_editor_core/ae_time_editor_launcher.h>
#include <audio_editor_core/ae_timing.h>
#include <audio_editor_core/ae_track_editor.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_receiver> project_receiver::make_shared(
    window_lifetime_id const &window_lifetime_id, track_editor *track_editor, playing_toggler *toggler,
    nudge_settings *nudge_settings, nudger *nudger, jumper *jumper, edge_editor *edge_editor,
    time_editor_launcher *time_editor_launcher, marker_editor *marker_editor,
    module_renaming_launcher *module_renaming_launcher, timing *timing, import_interactor *import_interactor,
    export_interactor *export_interactor, reverter *reverter) {
    return std::make_shared<project_receiver>(
        window_lifetime_id, track_editor, toggler, nudge_settings, nudger, jumper, edge_editor, time_editor_launcher,
        marker_editor, module_renaming_launcher, timing, import_interactor, export_interactor, reverter);
}

project_receiver::project_receiver(window_lifetime_id const &window_lifetime_id, track_editor *track_editor,
                                   playing_toggler *toggler, nudge_settings *nudge_settings, nudger *nudger,
                                   jumper *jumper, edge_editor *edge_editor, time_editor_launcher *time_editor_launcher,
                                   marker_editor *marker_editor, module_renaming_launcher *module_renaming_launcher,
                                   timing *timing, import_interactor *import_interactor,
                                   export_interactor *export_interactor, reverter *reverter)
    : _window_lifetime_id(window_lifetime_id),
      _editor(track_editor),
      _playing_toggler(toggler),
      _nudge_settings(nudge_settings),
      _nudger(nudger),
      _jumper(jumper),
      _edge_editor(edge_editor),
      _time_editor_launcher(time_editor_launcher),
      _marker_editor(marker_editor),
      _module_renaming_launcher(module_renaming_launcher),
      _timing(timing),
      _import_interactor(import_interactor),
      _export_interactor(export_interactor),
      _reverter(reverter) {
}

std::optional<action_id> project_receiver::receivable_id() const {
    return action_id{this->_window_lifetime_id};
}

std::optional<ae::action> project_receiver::to_action(ae::key const &key) const {
    std::optional<action_id> const action_id{this->_window_lifetime_id};

    switch (key) {
        case key::space:
            return action{action_kind::toggle_play, action_id, ""};
        case key::del:
            return action{action_kind::erase, action_id, ""};
        case key::a:
            return action{action_kind::drop_head, action_id, ""};
        case key::s:
            return action{action_kind::split, action_id, ""};
        case key::d:
            return action{action_kind::drop_tail, action_id, ""};
        case key::n:
            return action{action_kind::rotate_nudging_next_unit, action_id, ""};
        case key::shift_n:
            return action{action_kind::rotate_nudging_previous_unit, action_id, ""};
        case key::f:
            return action{action_kind::rotate_timing_fraction, action_id, ""};
        case key::t:
            return action{action_kind::begin_time_editing, action_id, ""};
        case key::left_bracket:
            return action{action_kind::set_begin_edge, action_id, ""};
        case key::right_bracket:
            return action{action_kind::set_end_edge, action_id, ""};
        case key::left:
            return action{action_kind::nudge_previous, action_id, ""};
        case key::right:
            return action{action_kind::nudge_next, action_id, ""};
        case key::shift_left:
            return action{action_kind::nudge_previous_more, action_id, ""};
        case key::shift_right:
            return action{action_kind::nudge_next_more, action_id, ""};
        case key::num_0:
            return action{action_kind::return_to_zero, action_id, ""};
        case key::num_1:
            return action{action_kind::go_to_marker, action_id, "0"};
        case key::num_2:
            return action{action_kind::go_to_marker, action_id, "1"};
        case key::num_3:
            return action{action_kind::go_to_marker, action_id, "2"};
        case key::num_4:
            return action{action_kind::go_to_marker, action_id, "3"};
        case key::num_5:
            return action{action_kind::go_to_marker, action_id, "4"};
        case key::num_6:
            return action{action_kind::go_to_marker, action_id, "5"};
        case key::num_7:
            return action{action_kind::go_to_marker, action_id, "6"};
        case key::num_8:
            return action{action_kind::go_to_marker, action_id, "7"};
        case key::num_9:
            return action{action_kind::go_to_marker, action_id, "8"};

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

void project_receiver::handle_action(ae::action const &action) const {
    auto const responding = this->responding_to_action(action);
    switch (responding) {
        case responding::accepting: {
            switch (action.kind) {
                case action_kind::toggle_play:
                    this->_playing_toggler->toggle_playing();
                    break;
                case action_kind::nudge_previous:
                    this->_nudger->nudge_previous(1);
                    break;
                case action_kind::nudge_next:
                    this->_nudger->nudge_next(1);
                    break;
                case action_kind::nudge_previous_more:
                    this->_nudger->nudge_previous(10);
                    break;
                case action_kind::nudge_next_more:
                    this->_nudger->nudge_next(10);
                    break;
                case action_kind::rotate_nudging_next_unit:
                    this->_nudge_settings->rotate_next_unit();
                    break;
                case action_kind::rotate_nudging_previous_unit:
                    this->_nudge_settings->rotate_previous_unit();
                    break;
                case action_kind::rotate_timing_fraction:
                    this->_timing->rotate_fraction();
                    break;
                case action_kind::jump_previous:
                    this->_jumper->jump_to_previous_edge();
                    break;
                case action_kind::jump_next:
                    this->_jumper->jump_to_next_edge();
                    break;
                case action_kind::jump_to_beginning:
                    this->_jumper->jump_to_beginning();
                    break;
                case action_kind::jump_to_end:
                    this->_jumper->jump_to_end();
                    break;
                case action_kind::drop_head:
                    this->_editor->drop_head();
                    break;
                case action_kind::split:
                    this->_editor->split();
                    break;
                case action_kind::drop_tail:
                    this->_editor->drop_tail();
                    break;
                case action_kind::erase:
                    this->_editor->erase();
                    break;
                case action_kind::insert_marker:
                    this->_marker_editor->insert_marker();
                    break;
                case action_kind::set_begin_edge:
                    this->_edge_editor->set_begin();
                    break;
                case action_kind::set_end_edge:
                    this->_edge_editor->set_end();
                    break;
                case action_kind::return_to_zero:
                    this->_jumper->return_to_zero();
                    break;
                case action_kind::go_to_marker:
                    this->_jumper->go_to_marker(std::stoi(action.value));
                    break;
                case action_kind::undo:
                    this->_reverter->undo();
                    break;
                case action_kind::redo:
                    this->_reverter->redo();
                    break;
                case action_kind::purge:
                    this->_reverter->purge();
                    break;
                case action_kind::select_file_for_import:
                    this->_import_interactor->select_file_for_import();
                    break;
                case action_kind::import_from_file:
                    this->_import_interactor->import_from_file(url::file_url(action.value));
                    break;
                case action_kind::select_file_for_export:
                    this->_export_interactor->select_file_for_export();
                    break;
                case action_kind::export_to_file:
                    this->_export_interactor->export_to_file(url::file_url(action.value));
                    break;
                case action_kind::cut:
                    this->_editor->cut_and_offset();
                    break;
                case action_kind::copy:
                    this->_editor->copy();
                    break;
                case action_kind::paste:
                    this->_editor->paste_and_offset();
                    break;
                case action_kind::begin_module_renaming:
                    this->_module_renaming_launcher->begin_module_renaming(action.value);
                    break;

                case action_kind::begin_time_editing:
                    this->_time_editor_launcher->begin_time_editing(std::nullopt);
                    break;
                case action_kind::select_time_unit:
                    this->_time_editor_launcher->begin_time_editing(std::stoi(action.value));
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

responding project_receiver::responding_to_action(ae::action const &action) const {
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
            return to_responding(this->_nudger->can_nudge());
        case action_kind::rotate_nudging_next_unit:
        case action_kind::rotate_nudging_previous_unit:
            return responding::accepting;
        case action_kind::rotate_timing_fraction:
            return responding::accepting;

        case action_kind::jump_previous:
            return to_responding(this->_jumper->can_jump_to_previous_edge());
        case action_kind::jump_next:
            return to_responding(this->_jumper->can_jump_to_next_edge());
        case action_kind::jump_to_beginning:
            return to_responding(this->_jumper->can_jump_to_beginnig());
        case action_kind::jump_to_end:
            return to_responding(this->_jumper->can_jump_to_end());

        case action_kind::drop_head:
            return to_responding(this->_editor->can_split());
        case action_kind::split:
            return to_responding(this->_editor->can_split());
        case action_kind::drop_tail:
            return to_responding(this->_editor->can_split());

        case action_kind::erase:
            return to_responding(this->_editor->can_erase());

        case action_kind::insert_marker:
            return to_responding(this->_marker_editor->can_insert_marker());

        case action_kind::return_to_zero:
            return to_responding(this->_jumper->can_return_to_zero());
        case action_kind::go_to_marker:
            return to_responding(this->_jumper->can_go_to_marker(std::stoi(action.value)));

        case action_kind::undo:
            return to_responding(this->_reverter->can_undo());
        case action_kind::redo:
            return to_responding(this->_reverter->can_redo());
        case action_kind::purge:
            return to_responding(this->_reverter->can_purge());

        case action_kind::select_file_for_import:
            return to_responding(this->_import_interactor->can_select_file_for_import());
        case action_kind::import_from_file:
            return to_responding(this->_import_interactor->can_import_from_file());
        case action_kind::select_file_for_export:
            return to_responding(this->_export_interactor->can_select_file_for_export());
        case action_kind::export_to_file:
            return to_responding(this->_export_interactor->can_export_to_file());

        case action_kind::cut:
            return to_responding(this->_editor->can_cut());
        case action_kind::copy:
            return to_responding(this->_editor->can_copy());
        case action_kind::paste:
            return to_responding(this->_editor->can_paste());

        case action_kind::begin_module_renaming:
            return to_responding(this->_module_renaming_launcher->can_begin_module_renaming());

        case action_kind::begin_time_editing:
            return to_responding(this->_time_editor_launcher->can_begin_time_editing());
        case action_kind::select_time_unit:
            return to_responding(this->_time_editor_launcher->can_begin_time_editing());

        case action_kind::set_begin_edge:
            return to_responding(this->_edge_editor->can_set_begin());
        case action_kind::set_end_edge:
            return to_responding(this->_edge_editor->can_set_end());

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
            return responding::fallthrough;
    }
}
