//
//  ae_ui_hierarchy.h
//

#pragma once

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_ui_editing_root_level.h>
#include <audio_editor_core/ae_ui_root_level.h>

namespace yas::ae::hierarchy {
[[nodiscard]] std::shared_ptr<ui_root_level> const &ui_root_level_for_view_id(uintptr_t const);
[[nodiscard]] std::shared_ptr<ui_editing_root_level> const &ui_editing_root_level_for_view_id(uintptr_t const);
}  // namespace yas::ae::hierarchy
