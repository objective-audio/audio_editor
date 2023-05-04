//
//  ae_modules_controller.cpp
//

#include "ae_modules_controller.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_json_utils.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <cpp_utils/yas_assertion.h>

#include <audio_editor_core/ae_range_selector.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<modules_controller> modules_controller::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    return std::make_shared<modules_controller>(project_lifetime->action_sender, project_lifetime->module_content_pool,
                                                project_lifetime->range_selector);
}

modules_controller::modules_controller(std::shared_ptr<project_action_sender> const &action_sender,
                                       std::shared_ptr<module_content_pool> const &content_pool,
                                       std::shared_ptr<range_selector> const &range_selector)
    : _action_sender(action_sender), _content_pool(content_pool), _range_selector(range_selector) {
}

void modules_controller::begin_range_selection(ui::point const &position) {
    auto const range_selector = this->_range_selector.lock();
    if (!range_selector) {
        return;
    }

    range_selector->begin(position);
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

void modules_controller::toggle_selection(std::size_t const idx) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto index = this->_module_index(idx)) {
        action_sender->send(editing_action_name::toggle_module_selection, std::move(index.value()));
    }
}

void modules_controller::begin_renaming(std::size_t const idx) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto index = this->_module_index(idx)) {
        action_sender->send(editing_action_name::begin_module_renaming, std::move(index.value()));
    }
}

std::optional<module_index> modules_controller::_module_index(std::size_t const idx) const {
    auto const module_indices = this->_module_indices({idx});
    if (module_indices.empty()) {
        return std::nullopt;
    } else {
        return module_indices.at(0);
    }
}

std::vector<module_index> modules_controller::_module_indices(std::vector<std::size_t> const &indices) const {
    auto const content_pool = this->_content_pool.lock();
    if (!content_pool) {
        assertion_failure_if_not_test();
        return {};
    }

    std::vector<module_index> result;

    auto const &contents = content_pool->elements();

    for (auto const &idx : indices) {
        if (idx < contents.size()) {
            auto const &content = contents.at(idx);
            if (content.has_value()) {
                result.emplace_back(content.value().index());
            }
        }
    }

    return result;
}
