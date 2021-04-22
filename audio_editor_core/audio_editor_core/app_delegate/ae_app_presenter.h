//
//  ae_app_presenter.h
//

#pragma once

#include <audio_editor_core/ae_project_pool_types.h>
#include <audio_editor_core/ae_ptr.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct app_presenter {
    enum class event_type {
        make_and_show_window_controller,
        dispose_window_controller,
    };

    struct event {
        event_type type;
        uintptr_t project_id;
    };

    app_presenter();
    explicit app_presenter(app_ptr const &);

    void add_project(url const &);
    [[nodiscard]] observing::syncable observe_event(std::function<void(event const &)> &&);

   private:
    app_ptr _app;
};
}  // namespace yas::ae
