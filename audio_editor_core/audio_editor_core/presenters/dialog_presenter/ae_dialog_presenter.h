//
//  ae_dialog_presenter.h
//

#pragma once

#include <audio_editor_core/ae_dialog_event.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct dialog_presenter final {
    [[nodiscard]] static std::shared_ptr<dialog_presenter> make_shared();

    dialog_presenter();

    void notify_event(dialog_event const &);

    observing::endable observe_event(std::function<void(dialog_event const &)> &&);

   private:
    observing::notifier_ptr<dialog_event> const _event_notifier;
};
}  // namespace yas::ae
