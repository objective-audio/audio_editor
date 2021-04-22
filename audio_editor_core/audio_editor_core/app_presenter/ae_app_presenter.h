//
//  ae_app_presenter.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_types.h>
#include <audio_editor_core/ae_project_pool_types.h>
#include <audio_editor_core/ae_ptr.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct app_presenter {
    using event_type = app_presenter_event_type;
    using event = app_presenter_event;

    app_presenter();
    explicit app_presenter(app_ptr const &);

    bool can_open_file_dialog() const;
    void open_file_dialog();

    void add_project(url const &);
    [[nodiscard]] observing::syncable observe_event(std::function<void(event const &)> &&);

   private:
    app_wptr _app;
    observing::notifier_ptr<event> const _event_notifier = observing::notifier<event>::make_shared();
};
}  // namespace yas::ae
