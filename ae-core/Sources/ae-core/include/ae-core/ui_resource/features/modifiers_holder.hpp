//
//  modifiers_holder.hpp
//

#pragma once

#include <ae-core/ui_resource/features/modifiers_holder_event.h>
#include <ui/yas_ui_umbrella.h>

#include <unordered_set>

namespace yas::ae {
struct modifiers_holder final {
    modifiers_holder(std::shared_ptr<ui::event_manager> const &);

    std::unordered_set<ae::modifier> const &modifiers() const;

    [[nodiscard]] observing::endable observe(std::function<void(ae::modifiers_holder_event const &)> &&);

   private:
    std::shared_ptr<ui::event_manager> const _event_manager;
    std::unordered_set<ae::modifier> _modifiers;
    observing::notifier_ptr<ae::modifiers_holder_event> const _modifier_notifier;
    observing::canceller_pool _pool;

    modifiers_holder(modifiers_holder const &) = delete;
    modifiers_holder(modifiers_holder &&) = delete;
    modifiers_holder &operator=(modifiers_holder const &) = delete;
    modifiers_holder &operator=(modifiers_holder &&) = delete;
};
}  // namespace yas::ae
