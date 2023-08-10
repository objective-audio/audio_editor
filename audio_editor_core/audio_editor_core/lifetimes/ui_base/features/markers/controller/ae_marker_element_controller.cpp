//
//  ae_marker_element_controller.cpp
//

#include "ae_marker_element_controller.hpp"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <cpp_utils/yas_assertion.h>

#include <audio_editor_core/ae_deselector.hpp>
#include <audio_editor_core/ae_range_selector.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<marker_element_controller> marker_element_controller::make_shared(
    project_lifetime_id const &lifetime_id) {
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id);
    return std::make_shared<marker_element_controller>(project_editing_lifetime->action_sender,
                                                       project_editing_lifetime->range_selector,
                                                       project_editing_lifetime->deselector);
}

marker_element_controller::marker_element_controller(std::shared_ptr<project_action_sender> const &action_sender,
                                                     std::shared_ptr<range_selector> const &range_selector,
                                                     std::shared_ptr<deselector> const &deselector)
    : _action_sender(action_sender), _range_selector(range_selector), _deselector(deselector) {
}

void marker_element_controller::deselect_all() {
    if (auto const deselector = this->_deselector.lock()) {
        deselector->deselect_all();
    }
}

void marker_element_controller::begin_range_selection(ui::point const &position) {
    if (auto const range_selector = this->_range_selector.lock()) {
        range_selector->begin(position);
    }
}

void marker_element_controller::toggle_selection(marker_index const &index) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::toggle_marker_selection, index);
}

void marker_element_controller::begin_renaming(marker_index const &index) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::begin_marker_renaming, index);
}
