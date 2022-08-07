//
//  ae_ui_hierarchy.h
//

#pragma once

#include <audio_editor_core/ae_hierarchy.h>

#include <audio_editor_core/ae_ui_base_lifetime.hpp>
#include <audio_editor_core/ae_ui_resource_lifetime.hpp>

namespace yas::ae {
struct ui_hierarchy final {
    [[nodiscard]] static std::shared_ptr<ui_resource_lifetime> const &resource_lifetime_for_window_lifetime_id(
        window_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<ui_base_lifetime> const &base_lifetime_for_window_lifetime_id(
        window_lifetime_id const &);
};
}  // namespace yas::ae
