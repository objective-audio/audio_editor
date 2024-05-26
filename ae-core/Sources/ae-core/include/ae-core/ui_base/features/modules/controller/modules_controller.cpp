//
//  modules_controller.cpp
//

#include "modules_controller.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project_editing/features/project_action_sender.h>
#include <cpp-utils/assertion.h>

#include <ae-core/project_editing/features/deselector.hpp>
#include <ae-core/project_editing/features/range_selector.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<modules_controller> modules_controller::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    return std::make_shared<modules_controller>(
        project_editing_lifetime->action_sender, project_editing_lifetime->module_content_pool,
        project_editing_lifetime->range_selector, project_editing_lifetime->deselector);
}

modules_controller::modules_controller(std::shared_ptr<project_action_sender> const &action_sender,
                                       std::shared_ptr<module_content_pool> const &content_pool,
                                       std::shared_ptr<range_selector> const &range_selector,
                                       std::shared_ptr<deselector> const &deselector)
    : _action_sender(action_sender),
      _content_pool(content_pool),
      _range_selector(range_selector),
      _deselector(deselector) {
}

void modules_controller::deselect_all() {
    if (auto const deselector = this->_deselector.lock()) {
        deselector->deselect_all();
    }
}

void modules_controller::begin_range_selection(ui::point const &position) {
    if (auto const range_selector = this->_range_selector.lock()) {
        range_selector->begin(position);
    }
}

void modules_controller::begin_selection() {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::begin_modules_selection);
}

void modules_controller::select(std::vector<std::size_t> const &indices) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    auto module_indices = this->_module_indices(indices);

    action_sender->send(editing_action_name::select_modules, std::move(module_indices));
}

void modules_controller::end_selection() {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    action_sender->send(editing_action_name::end_modules_selection);
}

void modules_controller::toggle_selection(std::size_t const content_idx) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto index = this->_module_index(content_idx)) {
        action_sender->send(editing_action_name::toggle_module_selection, std::move(index.value()));
    }
}

void modules_controller::begin_renaming(std::size_t const content_idx) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto module_index = this->_module_index(content_idx)) {
        action_sender->send(editing_action_name::begin_module_renaming, std::move(module_index.value()));
    }
}

std::optional<module_index> modules_controller::_module_index(std::size_t const idx) const {
    auto const module_indices = this->_module_indices({idx});
    if (module_indices.empty()) {
        return std::nullopt;
    } else {
        return *module_indices.begin();
    }
}

std::set<module_index> modules_controller::_module_indices(std::vector<std::size_t> const &content_indices) const {
    auto const content_pool = this->_content_pool.lock();
    if (!content_pool) {
        assertion_failure_if_not_test();
        return {};
    }

    std::set<module_index> module_indices;

    auto const &contents = content_pool->elements();

    for (auto const &idx : content_indices) {
        if (idx < contents.size()) {
            auto const &content = contents.at(idx);
            if (content.has_value()) {
                module_indices.emplace(content.value().index());
            }
        }
    }

    return module_indices;
}
