//
//  ae_app_dialog_sub_level_router.h
//

#pragma once

#include <audio_editor_core/ae_app_dialog_sub_level.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct app_dialog_sub_level_router final {
    [[nodiscard]] static std::shared_ptr<app_dialog_sub_level_router> make_shared();

    app_dialog_sub_level_router();

    [[nodiscard]] std::optional<app_dialog_sub_level> const &sub_level() const;

    void add_project_format_dialog();
    void remove_project_format_dialog();
    std::shared_ptr<project_setup_dialog_level> const &project_setup_dialog_level() const;

    observing::syncable observe(std::function<void(std::optional<app_dialog_sub_level> const &)> &&);

   private:
    observing::value::holder_ptr<std::optional<app_dialog_sub_level>> const _level;
};
}  // namespace yas::ae
