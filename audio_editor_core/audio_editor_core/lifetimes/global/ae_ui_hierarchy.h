//
//  ae_ui_hierarchy.h
//

#pragma once

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_ui_root_lifetime.h>

namespace yas::ae {
struct ui_hierarchy final {
    [[nodiscard]] static std::shared_ptr<ui_root_lifetime> const &root_lifetime_for_view_id(uintptr_t const);
};
}  // namespace yas::ae
