//
//  ae_modules_controller.cpp
//

#include "ae_modules_controller.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_json_utils.h>
#include <audio_editor_core/ae_project_action_controller.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<modules_controller> modules_controller::make_shared(
    window_lifetime_id const &window_lifetime_id, std::shared_ptr<module_location_pool> const &location_pool) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    return std::shared_ptr<modules_controller>(
        new modules_controller{project_lifetime->action_controller, location_pool});
}

modules_controller::modules_controller(std::shared_ptr<project_action_controller> const &action_controller,
                                       std::shared_ptr<module_location_pool> const &location_pool)
    : _action_controller(action_controller), _location_pool(location_pool) {
}

void modules_controller::select_module_at(std::size_t const idx) {
    auto const action_controller = this->_action_controller.lock();
    if (!action_controller) {
        assertion_failure_if_not_test();
        return;
    }

    auto const &locations = this->_location_pool->elements();
    if (idx < locations.size()) {
        auto const &location = locations.at(idx);
        if (location.has_value()) {
            action_controller->handle_action(action_kind::begin_module_renaming,
                                             to_json_string(location.value().range));
        }
    }
}
