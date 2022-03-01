//
//  ae_ui_hierarchy.mm
//

#include "ae_ui_hierarchy.h"
#include <audio_editor_core/ae_ui_editing_root_level_pool.h>
#include <audio_editor_core/ae_ui_root_level_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_level> const &hierarchy::ui_root_level_for_view_id(uintptr_t const view_id) {
    return app_level::global()->ui_root_level_pool->level_for_view_id(view_id);
}

std::shared_ptr<ui_editing_root_level> const &hierarchy::ui_editing_root_level_for_view_id(uintptr_t const view_id) {
    return ui_root_level_for_view_id(view_id)->ui_editing_root_level_pool->level();
}
