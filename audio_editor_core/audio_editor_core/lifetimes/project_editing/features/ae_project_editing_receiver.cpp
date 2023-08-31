//
//  ae_project_editing_receiver.cpp
//

#include "ae_project_editing_receiver.h"

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_editor.h>
#include <audio_editor_core/ae_export_interactor.h>
#include <audio_editor_core/ae_import_interactor.h>
#include <audio_editor_core/ae_jumper.h>
#include <audio_editor_core/ae_marker_editor.h>
#include <audio_editor_core/ae_module_editor.h>
#include <audio_editor_core/ae_module_renaming_opener.h>
#include <audio_editor_core/ae_nudger.h>
#include <audio_editor_core/ae_nudging.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_playing_toggler.h>
#include <audio_editor_core/ae_reverter.h>
#include <audio_editor_core/ae_time_editing_opener.h>
#include <audio_editor_core/ae_timing.h>

#include <audio_editor_core/ae_escaper.hpp>
#include <audio_editor_core/ae_marker_renaming_opener.hpp>
#include <audio_editor_core/ae_marker_selector.hpp>
#include <audio_editor_core/ae_module_selector.hpp>
#include <audio_editor_core/ae_project_settings_opener.hpp>

using namespace yas;
using namespace yas::ae;

project_editing_receiver::project_editing_receiver(
    project_lifetime_id const &project_lifetime_id, database *database, module_editor *module_editor,
    playing_toggler *toggler, nudging *nudging, nudger *nudger, jumper *jumper, edge_editor *edge_editor,
    time_editing_opener *time_editing_opener, marker_editor *marker_editor,
    module_renaming_opener *module_renaming_opener, marker_renaming_opener *marker_renaming_opener,
    project_settings_opener *settings_opener, timing *timing, import_interactor *import_interactor,
    export_interactor *export_interactor, reverter *reverter, module_selector *module_selector,
    marker_selector *marker_selector, escaper *escaper, pasteboard *pasteboard)
    : _project_lifetime_id(project_lifetime_id),
      _database(database),
      _module_editor(module_editor),
      _playing_toggler(toggler),
      _nudging(nudging),
      _nudger(nudger),
      _jumper(jumper),
      _edge_editor(edge_editor),
      _time_editing_opener(time_editing_opener),
      _marker_editor(marker_editor),
      _module_renaming_opener(module_renaming_opener),
      _marker_renaming_opener(marker_renaming_opener),
      _settings_opener(settings_opener),
      _timing(timing),
      _import_interactor(import_interactor),
      _export_interactor(export_interactor),
      _reverter(reverter),
      _module_selector(module_selector),
      _marker_selector(marker_selector),
      _escaper(escaper),
      _pasteboard(pasteboard) {
}

std::optional<action_id> project_editing_receiver::receivable_id() const {
    return action_id{this->_project_lifetime_id};
}

std::optional<ae::action> project_editing_receiver::to_action(ae::key const &key) const {
    switch (key) {
        case key::space:
            return action{editing_action_name::toggle_play};
        case key::del:
            return action{editing_action_name::erase};
        case key::a:
            return action{editing_action_name::drop_head};
        case key::s:
            return action{editing_action_name::split};
        case key::d:
            return action{editing_action_name::drop_tail};
        case key::n:
            return action{editing_action_name::rotate_nudging_next_unit};
        case key::shift_n:
            return action{editing_action_name::rotate_nudging_previous_unit};
        case key::f:
            return action{editing_action_name::rotate_timing_fraction};
        case key::t:
            return action{editing_action_name::begin_time_editing};
        case key::left_bracket:
            return action{editing_action_name::set_begin_edge};
        case key::right_bracket:
            return action{editing_action_name::set_end_edge};
        case key::left:
            return action{editing_action_name::nudge_previous};
        case key::right:
            return action{editing_action_name::nudge_next};
        case key::shift_left:
            return action{editing_action_name::nudge_previous_more};
        case key::shift_right:
            return action{editing_action_name::nudge_next_more};
        case key::num_0:
            return action{editing_action_name::return_to_zero};
        case key::num_1:
            return action{editing_action_name::go_to_marker, 0};
        case key::num_2:
            return action{editing_action_name::go_to_marker, 1};
        case key::num_3:
            return action{editing_action_name::go_to_marker, 2};
        case key::num_4:
            return action{editing_action_name::go_to_marker, 3};
        case key::num_5:
            return action{editing_action_name::go_to_marker, 4};
        case key::num_6:
            return action{editing_action_name::go_to_marker, 5};
        case key::num_7:
            return action{editing_action_name::go_to_marker, 6};
        case key::num_8:
            return action{editing_action_name::go_to_marker, 7};
        case key::num_9:
            return action{editing_action_name::go_to_marker, 8};
        case key::esc:
            return action{editing_action_name::escape};

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

void project_editing_receiver::receive(ae::action const &action) const {
    switch (this->receivable_state(action)) {
        case action_receivable_state::accepting: {
            switch (to_kind(action.name)) {
                case action_name_kind::editing: {
                    this->_database->suspend_saving([this, &action] {
                        switch (get<editing_action_name>(action.name)) {
                            case editing_action_name::toggle_play:
                                this->_playing_toggler->toggle_playing();
                                break;
                            case editing_action_name::nudge_previous:
                                this->_nudger->nudge_previous(1);
                                break;
                            case editing_action_name::nudge_next:
                                this->_nudger->nudge_next(1);
                                break;
                            case editing_action_name::nudge_previous_more:
                                this->_nudger->nudge_previous(10);
                                break;
                            case editing_action_name::nudge_next_more:
                                this->_nudger->nudge_next(10);
                                break;
                            case editing_action_name::rotate_nudging_next_unit:
                                this->_nudging->rotate_next_unit();
                                break;
                            case editing_action_name::rotate_nudging_previous_unit:
                                this->_nudging->rotate_previous_unit();
                                break;
                            case editing_action_name::rotate_timing_fraction:
                                this->_timing->rotate_fraction();
                                break;
                            case editing_action_name::jump_previous:
                                this->_jumper->jump_to_previous_edge();
                                break;
                            case editing_action_name::jump_next:
                                this->_jumper->jump_to_next_edge();
                                break;
                            case editing_action_name::jump_to_beginning:
                                this->_jumper->jump_to_beginning();
                                break;
                            case editing_action_name::jump_to_end:
                                this->_jumper->jump_to_end();
                                break;
                            case editing_action_name::drop_head:
                                this->_module_editor->drop_head();
                                break;
                            case editing_action_name::split:
                                this->_module_editor->split();
                                break;
                            case editing_action_name::drop_tail:
                                this->_module_editor->drop_tail();
                                break;
                            case editing_action_name::erase:
                                this->_module_editor->erase();
                                this->_marker_editor->erase();
                                break;
                            case editing_action_name::insert_marker:
                                this->_marker_editor->insert();
                                break;
                            case editing_action_name::set_begin_edge:
                                this->_edge_editor->set_begin();
                                break;
                            case editing_action_name::set_end_edge:
                                this->_edge_editor->set_end();
                                break;
                            case editing_action_name::return_to_zero:
                                this->_jumper->return_to_zero();
                                break;
                            case editing_action_name::go_to_marker:
                                this->_jumper->go_to_marker(action.integer_value());
                                break;
                            case editing_action_name::undo:
                                this->_reverter->undo();
                                break;
                            case editing_action_name::redo:
                                this->_reverter->redo();
                                break;
                            case editing_action_name::purge:
                                this->_reverter->purge();
                                break;
                            case editing_action_name::select_file_for_import:
                                this->_import_interactor->select_file_for_import();
                                break;
                            case editing_action_name::import_from_file:
                                this->_import_interactor->import_from_file(action.string_value());
                                break;
                            case editing_action_name::select_file_for_export:
                                this->_export_interactor->select_file_for_export();
                                break;
                            case editing_action_name::export_to_file:
                                this->_export_interactor->export_to_file(action.string_value());
                                break;
                            case editing_action_name::cut:
                                this->_pasteboard->clear();
                                this->_module_editor->cut();
                                this->_marker_editor->cut();
                                break;
                            case editing_action_name::copy:
                                this->_pasteboard->clear();
                                this->_module_editor->copy();
                                this->_marker_editor->copy();
                                break;
                            case editing_action_name::paste:
                                this->_module_editor->paste();
                                this->_marker_editor->paste();
                                break;
                            case editing_action_name::escape:
                                this->_escaper->espace();
                                break;
                            case editing_action_name::begin_modules_selection:
                                this->_module_selector->begin_selection();
                                break;
                            case editing_action_name::select_modules:
                                this->_module_selector->select(action.module_index_set_value());
                                break;
                            case editing_action_name::end_modules_selection:
                                this->_module_selector->end_selection();
                                break;
                            case editing_action_name::toggle_module_selection:
                                this->_module_selector->toggle(action.module_index_value());
                                break;
                            case editing_action_name::begin_module_renaming:
                                this->_module_renaming_opener->begin_module_renaming(action.module_index_value());
                                break;
                            case editing_action_name::begin_markers_selection:
                                this->_marker_selector->begin_selection();
                                break;
                            case editing_action_name::select_markers:
                                this->_marker_selector->select(action.marker_index_set_value());
                                break;
                            case editing_action_name::end_markers_selection:
                                this->_marker_selector->end_selection();
                                break;
                            case editing_action_name::toggle_marker_selection:
                                this->_marker_selector->toggle(action.marker_index_value());
                                break;
                            case editing_action_name::begin_marker_renaming:
                                this->_marker_renaming_opener->begin_renaming(action.marker_index_value());
                                break;

                            case editing_action_name::begin_time_editing:
                                this->_time_editing_opener->begin_time_editing();
                                break;

                            case editing_action_name::open_project_settings:
                                this->_settings_opener->open();
                                break;
                        }
                    });
                } break;

                case action_name_kind::time_editing:
                    break;
            }
        } break;

        case action_receivable_state::blocking:
        case action_receivable_state::fallthrough:
            break;
    }
}

action_receivable_state project_editing_receiver::receivable_state(ae::action const &action) const {
    switch (to_kind(action.name)) {
        case action_name_kind::editing: {
            static auto const to_state = [](bool const &flag) {
                return flag ? action_receivable_state::accepting : action_receivable_state::blocking;
            };

            switch (get<editing_action_name>(action.name)) {
                case editing_action_name::toggle_play:
                    return action_receivable_state::accepting;
                case editing_action_name::nudge_previous:
                case editing_action_name::nudge_next:
                case editing_action_name::nudge_previous_more:
                case editing_action_name::nudge_next_more:
                    return to_state(this->_nudger->can_nudge());
                case editing_action_name::rotate_nudging_next_unit:
                case editing_action_name::rotate_nudging_previous_unit:
                    return action_receivable_state::accepting;
                case editing_action_name::rotate_timing_fraction:
                    return action_receivable_state::accepting;

                case editing_action_name::jump_previous:
                    return to_state(this->_jumper->can_jump_to_previous_edge());
                case editing_action_name::jump_next:
                    return to_state(this->_jumper->can_jump_to_next_edge());
                case editing_action_name::jump_to_beginning:
                    return to_state(this->_jumper->can_jump_to_beginnig());
                case editing_action_name::jump_to_end:
                    return to_state(this->_jumper->can_jump_to_end());

                case editing_action_name::drop_head:
                    return to_state(this->_module_editor->can_split());
                case editing_action_name::split:
                    return to_state(this->_module_editor->can_split());
                case editing_action_name::drop_tail:
                    return to_state(this->_module_editor->can_split());

                case editing_action_name::erase:
                    return to_state(this->_module_editor->can_erase() || this->_marker_editor->can_erase());

                case editing_action_name::insert_marker:
                    return to_state(this->_marker_editor->can_insert());

                case editing_action_name::return_to_zero:
                    return to_state(this->_jumper->can_return_to_zero());
                case editing_action_name::go_to_marker:
                    return to_state(this->_jumper->can_go_to_marker(action.integer_value()));

                case editing_action_name::undo:
                    return to_state(this->_reverter->can_undo());
                case editing_action_name::redo:
                    return to_state(this->_reverter->can_redo());
                case editing_action_name::purge:
                    return to_state(this->_reverter->can_purge());

                case editing_action_name::select_file_for_import:
                    return to_state(this->_import_interactor->can_select_file_for_import());
                case editing_action_name::import_from_file:
                    return to_state(this->_import_interactor->can_import_from_file());
                case editing_action_name::select_file_for_export:
                    return to_state(this->_export_interactor->can_select_file_for_export());
                case editing_action_name::export_to_file:
                    return to_state(this->_export_interactor->can_export_to_file());

                case editing_action_name::cut:
                    return to_state(this->_module_editor->can_cut() || this->_marker_editor->can_cut());
                case editing_action_name::copy:
                    return to_state(this->_module_editor->can_copy() || this->_marker_editor->can_copy());
                case editing_action_name::paste:
                    return to_state(this->_module_editor->can_paste() || this->_marker_editor->can_paste());
                case editing_action_name::escape:
                    return to_state(this->_escaper->can_escape());

                case editing_action_name::begin_modules_selection:
                    return to_state(this->_module_selector->can_select());
                case editing_action_name::select_modules:
                    return to_state(this->_module_selector->can_select());
                case editing_action_name::end_modules_selection:
                    return to_state(this->_module_selector->can_select());
                case editing_action_name::toggle_module_selection:
                    return to_state(this->_module_selector->can_toggle());
                case editing_action_name::begin_module_renaming:
                    return to_state(this->_module_renaming_opener->can_begin_module_renaming());
                case editing_action_name::begin_marker_renaming:
                    return to_state(this->_marker_renaming_opener->can_begin_renaming());
                case editing_action_name::begin_markers_selection:
                    return to_state(this->_marker_selector->can_select());
                case editing_action_name::select_markers:
                    return to_state(this->_marker_selector->can_select());
                case editing_action_name::end_markers_selection:
                    return to_state(this->_marker_selector->can_select());
                case editing_action_name::toggle_marker_selection:
                    return to_state(this->_marker_selector->can_toggle());

                case editing_action_name::begin_time_editing:
                    return to_state(this->_time_editing_opener->can_begin_time_editing());

                case editing_action_name::set_begin_edge:
                    return to_state(this->_edge_editor->can_set_begin());
                case editing_action_name::set_end_edge:
                    return to_state(this->_edge_editor->can_set_end());

                case editing_action_name::open_project_settings:
                    return to_state(this->_settings_opener->can_open());
            }
        }

        case action_name_kind::time_editing:
            return action_receivable_state::fallthrough;
    }
}
