//
//  ae_app_presenter.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_app_presenter_types.h>

namespace yas::ae {
struct app_presenter {
    using event_type = app_presenter_event_type;
    using event = app_presenter_event;

    app_presenter();
    explicit app_presenter(std::shared_ptr<app_presenter_project_pool_interface> const &);

    [[nodiscard]] bool can_open_file_dialog() const;
    void open_file_dialog();
    void select_file(url const &);

    [[nodiscard]] observing::syncable observe_event(std::function<void(event const &)> &&);

   private:
    std::weak_ptr<app_presenter_project_pool_interface> _project_pool;
    observing::notifier_ptr<event> const _event_notifier = observing::notifier<event>::make_shared();
};
}  // namespace yas::ae
