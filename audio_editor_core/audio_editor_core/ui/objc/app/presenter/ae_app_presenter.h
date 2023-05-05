//
//  ae_app_presenter.h
//

#pragma once

#include <audio_editor_core/ae_app_modal_sub_lifetime.h>
#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_app_presenter_event.h>

namespace yas::ae {
class project_opener;
class app_modal_lifecycle;

struct app_presenter final {
    [[nodiscard]] static std::shared_ptr<app_presenter> make_shared();

    app_presenter(std::shared_ptr<project_lifecycle_for_app_presenter> const &,
                  std::shared_ptr<project_settings_lifecycle_for_app_presenter> const &,
                  std::shared_ptr<app_settings_lifecycle_for_app_presenter> const &,
                  std::shared_ptr<app_modal_lifecycle> const &, std::shared_ptr<project_opener> const &);

    [[nodiscard]] bool can_open_dialog() const;
    void open_project_setup_dialog();

    [[nodiscard]] bool can_open_app_settings() const;
    void open_app_settings();

    [[nodiscard]] observing::syncable observe_window(std::function<void(app_presenter_window_event const &)> &&);
    [[nodiscard]] observing::syncable observe_dialog(
        std::function<void(std::optional<app_modal_sub_lifetime> const &)> &&);

   private:
    std::weak_ptr<project_lifecycle_for_app_presenter> const _project_lifecycle;
    std::weak_ptr<project_settings_lifecycle_for_app_presenter> const _project_settings_lifecycle;
    std::weak_ptr<app_settings_lifecycle_for_app_presenter> const _app_settings_lifecycle;
    std::weak_ptr<app_modal_lifecycle> const _app_modal_lifecycle;
    std::weak_ptr<project_opener> const _project_opener;

    app_presenter(app_presenter const &) = delete;
    app_presenter(app_presenter &&) = delete;
    app_presenter &operator=(app_presenter const &) = delete;
    app_presenter &operator=(app_presenter &&) = delete;
};
}  // namespace yas::ae
