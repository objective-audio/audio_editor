//
//  ae_modifiers_holder.hpp
//

#pragma once

#include <audio_editor_core/ae_modifier_types.h>
#include <ui/yas_ui_umbrella.h>

#include <unordered_set>

namespace yas::ae {
struct modifiers_holder final {
    modifiers_holder(std::shared_ptr<ui::event_manager> const &);

    std::unordered_set<ae::modifier> const &modifiers() const;

    [[nodiscard]] observing::endable observe_modifier(std::function<void(ae::modifier_event const &)> &&);

   private:
    std::shared_ptr<ui::event_manager> const _event_manager;
    std::unordered_set<ae::modifier> _modifiers;
    observing::notifier_ptr<ae::modifier_event> const _modifier_notifier;
    observing::canceller_pool _pool;
};
}  // namespace yas::ae
