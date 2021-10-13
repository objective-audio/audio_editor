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
    return std::shared_ptr<action_controller>(new action_controller{project});
}

action_controller::action_controller(std::shared_ptr<project> const &project) : _project(project) {
}

void action_controller::handle_action(action const action) {
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
        case action::erase_and_offset:
            project->erase_and_offset();
            break;
        case action::insert_marker:
            project->insert_marker();
            break;
        case action::return_to_zero:
            project->return_to_zero();
            break;
        case action::go_to_marker_1:
            project->go_to_marker(0);
            break;
        case action::go_to_marker_2:
            project->go_to_marker(1);
            break;
        case action::go_to_marker_3:
            project->go_to_marker(2);
            break;
        case action::go_to_marker_4:
            project->go_to_marker(3);
            break;
        case action::go_to_marker_5:
            project->go_to_marker(4);
            break;
        case action::go_to_marker_6:
            project->go_to_marker(5);
            break;
        case action::go_to_marker_7:
            project->go_to_marker(6);
            break;
        case action::go_to_marker_8:
            project->go_to_marker(7);
            break;
        case action::go_to_marker_9:
            project->go_to_marker(8);
            break;
    }
}
