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
    return std::make_shared<modules_controller>(project_lifetime->action_sender,
                                                project_lifetime->module_location_pool);
}

modules_controller::modules_controller(std::shared_ptr<project_action_sender> const &action_sender,
                                       std::shared_ptr<module_location_pool> const &location_pool)
    : _action_sender(action_sender), _location_pool(location_pool) {
}

void modules_controller::select_module_at(std::size_t const idx) {
    auto const action_sender = this->_action_sender.lock();
    auto const location_pool = this->_location_pool.lock();
    if (!action_sender || !location_pool) {
        assertion_failure_if_not_test();
        return;
    }

    auto const &locations = location_pool->elements();
    if (idx < locations.size()) {
        auto const &location = locations.at(idx);
        if (location.has_value()) {
            action_sender->send(editing_action_name::begin_module_renaming, location.value().range);
        }
    }
}
