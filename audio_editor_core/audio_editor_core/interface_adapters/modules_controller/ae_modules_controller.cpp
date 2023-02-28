//
//  ae_modules_controller.cpp
//

#include "ae_modules_controller.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_json_utils.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<modules_controller> modules_controller::make_shared(window_lifetime_id const &window_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    return std::make_shared<modules_controller>(project_lifetime->action_sender, project_lifetime->module_content_pool);
}

modules_controller::modules_controller(std::shared_ptr<project_action_sender> const &action_sender,
                                       std::shared_ptr<module_content_pool> const &content_pool)
    : _action_sender(action_sender), _content_pool(content_pool) {
}

void modules_controller::select(std::size_t const index) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto index = this->index_at(idx)) {
        action_sender->send(editing_action_name::select_module, std::move(index.value()));
    }
}

void modules_controller::toggle_selection(std::size_t const idx) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto index = this->index_at(idx)) {
        action_sender->send(editing_action_name::toggle_module_selection, std::move(index.value()));
    }
}

void modules_controller::begin_renaming(std::size_t const idx) {
    auto const action_sender = this->_action_sender.lock();
    if (!action_sender) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto index = this->index_at(idx)) {
        action_sender->send(editing_action_name::begin_module_renaming, std::move(index.value()));
    }
}

std::optional<module_index> modules_controller::index_at(std::size_t const idx) const {
    auto const content_pool = this->_content_pool.lock();
    if (!content_pool) {
        assertion_failure_if_not_test();
        return std::nullopt;
    }

    auto const &contents = content_pool->elements();
    if (idx < contents.size()) {
        auto const &content = contents.at(idx);
        if (content.has_value()) {
            return content.value().index();
        }
    }

    return std::nullopt;
}
