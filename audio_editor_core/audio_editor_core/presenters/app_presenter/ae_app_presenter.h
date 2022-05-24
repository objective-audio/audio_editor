//
//  ae_app_presenter.h
//

#pragma once

#include <audio_editor_core/ae_app_dialog_content.h>
#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_app_presenter_types.h>

namespace yas::ae {
class project_preparer;
class app_dialog_level_router;

struct app_presenter final {
    [[nodiscard]] static std::shared_ptr<app_presenter> make_shared();

    app_presenter(std::shared_ptr<project_level_router_for_app_presenter> const &,
                  std::shared_ptr<app_dialog_level_router> const &, std::shared_ptr<project_preparer> const &);

    [[nodiscard]] bool can_open_audio_file_dialog() const;
    void open_audio_file_dialog();
    void did_close_audio_file_dialog();
    void select_audio_file(url const &);

    [[nodiscard]] observing::syncable observe_event(std::function<void(app_presenter_event const &)> &&);
    [[nodiscard]] observing::syncable observe_dialog(std::function<void(std::optional<app_dialog_content>)> &&);

   private:
    std::weak_ptr<project_level_router_for_app_presenter> const _project_level_router;
    std::weak_ptr<app_dialog_level_router> const _dialog_level_router;
    std::weak_ptr<project_preparer> const _project_preparer;

    app_presenter(app_presenter const &) = delete;
    app_presenter(app_presenter &&) = delete;
    app_presenter &operator=(app_presenter const &) = delete;
    app_presenter &operator=(app_presenter &&) = delete;
};
}  // namespace yas::ae
