//
//  ae_ui_hierarchy.mm
//

#include "ae_ui_hierarchy.h"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_ui_base_lifecycle.hpp>
#include <audio_editor_core/ae_ui_resource_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_resource_lifetime> const &ui_hierarchy::resource_lifetime_for_window_lifetime_id(
    window_lifetime_id const &window_lifetime_id) {
    return hierarchy::app_lifetime()->ui_resource_lifecycle->lifetime_for_window_lifetime_id(window_lifetime_id);
}

std::shared_ptr<ui_base_lifetime> const &ui_hierarchy::base_lifetime_for_window_lifetime_id(
    window_lifetime_id const &window_lifetime_id) {
    auto const &resource_lifetime = resource_lifetime_for_window_lifetime_id(window_lifetime_id);
    return resource_lifetime->base_lifecycle->lifetime();
}
