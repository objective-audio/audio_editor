//
//  ae_app_presenter.h
//

#pragma once

#include <audio_editor_core/ae_app_dialog_sub_level.h>
#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_app_presenter_types.h>

namespace yas::ae {
class project_preparer;
class app_dialog_sub_level_router;
class project_format;

struct app_presenter final {
    [[nodiscard]] static std::shared_ptr<app_presenter> make_shared();

    app_presenter(std::shared_ptr<project_level_router_for_app_presenter> const &,
                  std::shared_ptr<app_dialog_sub_level_router> const &, std::shared_ptr<project_preparer> const &);

    [[nodiscard]] bool can_open_dialog() const;
    void open_project_setup_dialog();

    [[nodiscard]] observing::syncable observe_window(std::function<void(app_presenter_window_event const &)> &&);
    [[nodiscard]] observing::syncable observe_dialog(
        std::function<void(std::optional<app_dialog_sub_level> const &)> &&);

   private:
    std::weak_ptr<project_level_router_for_app_presenter> const _project_level_router;
    std::weak_ptr<app_dialog_sub_level_router> const _dialog_level_router;
    std::weak_ptr<project_preparer> const _project_preparer;

    app_presenter(app_presenter const &) = delete;
    app_presenter(app_presenter &&) = delete;
    app_presenter &operator=(app_presenter const &) = delete;
    app_presenter &operator=(app_presenter &&) = delete;
};
}  // namespace yas::ae
