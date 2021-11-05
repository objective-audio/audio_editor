//
//  ae_action_controller.cpp
//

#include "ae_action_controller.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_controller> action_controller::make_shared(std::string const &project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return std::shared_ptr<action_controller>(new action_controller{project->editor()});
}

action_controller::action_controller(std::shared_ptr<project_editor_for_action_controller> const &editor)
    : _project_editor(editor) {
}

void action_controller::handle_action(action const action) {
    auto const editor = this->_project_editor.lock();
    if (!editor) {
        return;
    }

    switch (action) {
        case action::toggle_play:
            editor->toggle_playing();
            break;
        case action::nudge_previous:
            editor->nudge_previous();
            break;
        case action::nudge_next:
            editor->nudge_next();
            break;
        case action::jump_previous:
            editor->jump_to_previous_edge();
            break;
        case action::jump_next:
            editor->jump_to_next_edge();
            break;
        case action::drop_head_and_offset:
            editor->drop_head_and_offset();
            break;
        case action::split:
            editor->split();
            break;
        case action::drop_tail_and_offset:
            editor->drop_tail_and_offset();
            break;
        case action::erase_and_offset:
            editor->erase_and_offset();
            break;
        case action::insert_marker:
            editor->insert_marker();
            break;
        case action::return_to_zero:
            editor->return_to_zero();
            break;
        case action::go_to_marker_1:
            editor->go_to_marker(0);
            break;
        case action::go_to_marker_2:
            editor->go_to_marker(1);
            break;
        case action::go_to_marker_3:
            editor->go_to_marker(2);
            break;
        case action::go_to_marker_4:
            editor->go_to_marker(3);
            break;
        case action::go_to_marker_5:
            editor->go_to_marker(4);
            break;
        case action::go_to_marker_6:
            editor->go_to_marker(5);
            break;
        case action::go_to_marker_7:
            editor->go_to_marker(6);
            break;
        case action::go_to_marker_8:
            editor->go_to_marker(7);
            break;
        case action::go_to_marker_9:
            editor->go_to_marker(8);
            break;
        case action::undo:
            editor->undo();
            break;
        case action::redo:
            editor->redo();
            break;
    }
}
