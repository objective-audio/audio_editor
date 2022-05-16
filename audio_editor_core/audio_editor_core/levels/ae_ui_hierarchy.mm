//
//  ae_ui_hierarchy.mm
//

#include "ae_ui_hierarchy.h"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_ui_root_level_router.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_level> const &ui_hierarchy::root_level_for_view_id(uintptr_t const view_id) {
    return hierarchy::app_level()->ui_root_level_router->level_for_view_id(view_id);
}
