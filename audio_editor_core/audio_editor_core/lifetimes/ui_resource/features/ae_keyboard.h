//
//  ae_keyboard.h
//

#pragma once

#include <audio_editor_core/ae_keyboard_types.h>
#include <ui/yas_ui_umbrella.h>

#include <memory>
#include <unordered_set>

namespace yas::ae {
struct keyboard final {
    [[nodiscard]] observing::endable observe_key(std::function<void(ae::key const &)> &&);
    [[nodiscard]] observing::endable observe_modifier(std::function<void(ae::modifier_event const &)> &&);

    [[nodiscard]] static std::shared_ptr<keyboard> make_shared(std::shared_ptr<ui::event_manager> const &);

   private:
    std::shared_ptr<ui::event_manager> const _event_manager;
    std::unordered_set<ae::modifier> _modifiers;
    observing::notifier_ptr<ae::key> const _key_notifier;
    observing::notifier_ptr<ae::modifier_event> const _modifier_notifier;
    observing::canceller_pool _pool;

    explicit keyboard(std::shared_ptr<ui::event_manager> const &);
};
}  // namespace yas::ae
