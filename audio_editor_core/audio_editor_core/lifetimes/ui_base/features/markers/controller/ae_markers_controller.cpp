//
//  ae_markers_controller.cpp
//

#include "ae_markers_controller.hpp"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<markers_controller> markers_controller::make_shared(project_lifetime_id const &lifetime_id) {
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id);
    return std::make_shared<markers_controller>(project_editing_lifetime->action_sender);
}

markers_controller::markers_controller(std::shared_ptr<project_action_sender> const &action_sender)
    : _action_sender(action_sender) {
}

void markers_controller::select(std::vector<marker_index> const &marker_indices) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::select_markers, marker_indices);
}
