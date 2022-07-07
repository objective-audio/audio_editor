//
//  ae_ui_hierarchy.mm
//

#include "ae_ui_hierarchy.h"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_ui_root_lifecycle.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_lifetime> const &ui_hierarchy::root_lifetime_for_window_lifetime_id(
    window_lifetime_id const &window_lifetime_id) {
    return hierarchy::app_lifetime()->ui_root_lifecycle->lifetime_for_window_lifetime_id(window_lifetime_id);
}
