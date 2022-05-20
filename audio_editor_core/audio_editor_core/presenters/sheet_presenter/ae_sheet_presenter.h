//
//  ae_sheet_presenter.h
//

#pragma once

#include <audio_editor_core/ae_sheet_event.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct sheet_presenter final {
    [[nodiscard]] static std::shared_ptr<sheet_presenter> make_shared();

    sheet_presenter();

    void notify_event(sheet_event const &);

    observing::endable observe_event(std::function<void(sheet_event const &)> &&);

   private:
    observing::notifier_ptr<sheet_event> const _event_notifier;
};
}  // namespace yas::ae
