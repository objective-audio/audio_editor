//
//  ae_marker_element_controller.cpp
//

#include "ae_marker_element_controller.hpp"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<marker_element_controller> marker_element_controller::make_shared(
    window_lifetime_id const &lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id);
    return std::make_shared<marker_element_controller>(project_lifetime->action_sender);
}

marker_element_controller::marker_element_controller(std::shared_ptr<project_action_sender> const &action_sender)
    : _action_sender(action_sender) {
}

void marker_element_controller::select_marker_at(marker_index const &index) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::select_marker, index);
}

void marker_element_controller::toggle_marker_selection_at(marker_index const &index) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::toggle_marker_selection, index);
}

void marker_element_controller::begin_marker_renaming_at(marker_index const &index) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::begin_marker_renaming, index);
}
