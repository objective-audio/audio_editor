//
//  ae_ui_hierarchy.mm
//

#include "ae_ui_hierarchy.h"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_ui_root_lifecycle.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_lifetime> const &ui_hierarchy::root_lifetime_for_view_id(uintptr_t const view_id) {
    return hierarchy::app_lifetime()->ui_root_lifecycle->lifetime_for_view_id(view_id);
}
