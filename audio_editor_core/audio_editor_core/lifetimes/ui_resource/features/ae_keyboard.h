//
//  ae_keyboard.h
//

#pragma once

#include <audio_editor_core/ae_keyboard_types.h>
#include <audio_editor_core/ae_modifier_types.h>
#include <ui/yas_ui_umbrella.h>

#include <memory>
#include <unordered_set>

namespace yas::ae {
class modifiers_holder;

struct keyboard final {
    keyboard(std::shared_ptr<ui::event_manager> const &, modifiers_holder *);

    [[nodiscard]] observing::endable observe(std::function<void(ae::key const &)> &&);

   private:
    std::shared_ptr<ui::event_manager> const _event_manager;
    modifiers_holder *const _modifiers_holder;
    observing::notifier_ptr<ae::key> const _key_notifier;
    observing::canceller_pool _pool;
};
}  // namespace yas::ae
