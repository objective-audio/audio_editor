//
//  ae_ui_root_dependency.h
//

#pragma once

#include <audio_editor_core/ae_keyboard_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class project_view_presenter;

struct ui_root_keyboard_interface {
    virtual ~ui_root_keyboard_interface() = default;

    [[nodiscard]] virtual observing::endable observe(std::function<void(ae::key const &)> &&) = 0;
};
}  // namespace yas::ae
