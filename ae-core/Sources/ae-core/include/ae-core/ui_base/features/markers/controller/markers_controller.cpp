//
//  markers_controller.cpp
//

#include "markers_controller.hpp"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project_editing/features/project_action_sender.h>
#include <cpp-utils/assertion.h>

#include <ae-core/project_editing/features/deselector.hpp>
#include <ae-core/project_editing/features/range_selector.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<markers_controller> markers_controller::make_shared(project_lifetime_id const &lifetime_id) {
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id);
    return std::make_shared<markers_controller>(project_editing_lifetime->action_sender,
                                                project_editing_lifetime->range_selector,
                                                project_editing_lifetime->deselector);
}

markers_controller::markers_controller(std::shared_ptr<project_action_sender> const &action_sender,
                                       std::shared_ptr<range_selector> const &range_selector,
                                       std::shared_ptr<deselector> const &deselector)
    : _action_sender(action_sender), _range_selector(range_selector), _deselector(deselector) {
}

void markers_controller::deselect_all() {
    if (auto const deselector = this->_deselector.lock()) {
        deselector->deselect_all();
    }
}

void markers_controller::begin_range_selection(ui::point const &position) {
    if (auto const range_selector = this->_range_selector.lock()) {
        range_selector->begin(position);
    }
}

void markers_controller::begin_selection() {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::begin_markers_selection);
}

void markers_controller::select(std::set<marker_index> const &marker_indices) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::select_markers, marker_indices);
}

void markers_controller::end_selection() {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::end_markers_selection);
}

void markers_controller::toggle_selection(marker_index const &index) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::toggle_marker_selection, index);
}

void markers_controller::begin_renaming(marker_index const &index) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::begin_marker_renaming, index);
}
