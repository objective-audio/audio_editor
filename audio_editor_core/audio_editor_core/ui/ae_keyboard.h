//
//  ae_keyboard.h
//

#pragma once

#include <audio_editor_core/ae_ui_root_dependency.h>
#include <ui/yas_ui_umbrella.h>

#include <memory>
#include <unordered_set>

namespace yas::ae {
struct keyboard : ui_root_keyboard_interface {
    [[nodiscard]] observing::endable observe(std::function<void(ae::key const &)> &&) override;

    [[nodiscard]] static std::shared_ptr<keyboard> make_shared(std::shared_ptr<ui::event_manager> const &);

   private:
    std::shared_ptr<ui::event_manager> const _event_manager;
    std::unordered_set<ui::modifier_flags> _modifiers;
    observing::notifier_ptr<ae::key> const _notifier;
    observing::canceller_pool _pool;

    explicit keyboard(std::shared_ptr<ui::event_manager> const &);
};
}  // namespace yas::ae
