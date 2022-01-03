//
//  ae_ui_editing_root_dependency.h
//

#pragma once

#include <audio_editor_core/ae_keyboard_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct keyboard_for_ui_root {
    virtual ~keyboard_for_ui_root() = default;

    [[nodiscard]] virtual observing::endable observe_key(std::function<void(ae::key const &)> &&) = 0;
};
}  // namespace yas::ae
