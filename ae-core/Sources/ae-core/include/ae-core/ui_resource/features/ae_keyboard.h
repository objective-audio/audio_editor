//
//  ae_keyboard.h
//

#pragma once

#include <ae-core/ui_resource/features/ae_key.h>
#include <ae-core/ui_resource/features/ae_modifier.h>
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

    keyboard(keyboard const &) = delete;
    keyboard(keyboard &&) = delete;
    keyboard &operator=(keyboard const &) = delete;
    keyboard &operator=(keyboard &&) = delete;
};
}  // namespace yas::ae
