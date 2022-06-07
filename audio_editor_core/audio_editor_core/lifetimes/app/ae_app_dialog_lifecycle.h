//
//  ae_app_dialog_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_app_dialog_sub_lifetime.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct app_dialog_lifecycle final {
    [[nodiscard]] static std::shared_ptr<app_dialog_lifecycle> make_shared();

    app_dialog_lifecycle();

    [[nodiscard]] std::optional<app_dialog_sub_lifetime> const &current() const;

    void add_project_format_dialog();
    void remove_project_format_dialog();
    std::shared_ptr<project_setup_dialog_lifetime> const &project_setup_dialog_lifetime() const;

    observing::syncable observe(std::function<void(std::optional<app_dialog_sub_lifetime> const &)> &&);

   private:
    observing::value::holder_ptr<std::optional<app_dialog_sub_lifetime>> const _current;
};
}  // namespace yas::ae
