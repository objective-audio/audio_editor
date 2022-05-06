//
//  ae_modules_controller.cpp
//

#include "ae_modules_controller.h"

#include <audio_editor_core/ae_action_controller.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<modules_controller> modules_controller::make_shared(
    project_id const &project_id, std::shared_ptr<module_location_pool> const &location_pool) {
    auto const action_controller = action_controller::make_shared(project_id);
    return std::shared_ptr<modules_controller>(new modules_controller{action_controller, location_pool});
}

modules_controller::modules_controller(std::shared_ptr<action_controller> const &action_controller,
                                       std::shared_ptr<module_location_pool> const &location_pool)
    : _action_controller(action_controller), _location_pool(location_pool) {
}

void modules_controller::select_module_at(std::size_t const idx) {
    auto const &locations = this->_location_pool->elements();
    if (idx < locations.size()) {
        auto const &location = locations.at(idx);
        if (location.has_value()) {
            auto const &range = location.value().range;
            std::string value = std::to_string(range.frame) + "," + std::to_string(range.length);
            this->_action_controller->handle_action({action_kind::begin_module_renaming, std::move(value)});
        }
    }
}
