//
//  ae_app_presenter.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_app_presenter_types.h>

namespace yas::ae {
class project_preparer;

struct app_presenter final {
    [[nodiscard]] static std::shared_ptr<app_presenter> make_shared();

    app_presenter(std::shared_ptr<project_level_router_for_app_presenter> const &,
                  std::shared_ptr<project_preparer> const &);

    [[nodiscard]] bool can_open_file_dialog() const;
    void open_file_dialog();
    void select_file(url const &);

    [[nodiscard]] observing::syncable observe_event(std::function<void(app_presenter_event const &)> &&);

   private:
    std::weak_ptr<project_level_router_for_app_presenter> _project_pool;
    std::weak_ptr<project_preparer> const _project_preparer;
    observing::notifier_ptr<app_presenter_event> const _event_notifier =
        observing::notifier<app_presenter_event>::make_shared();

    app_presenter(app_presenter const &) = delete;
    app_presenter(app_presenter &&) = delete;
    app_presenter &operator=(app_presenter const &) = delete;
    app_presenter &operator=(app_presenter &&) = delete;
};
}  // namespace yas::ae
