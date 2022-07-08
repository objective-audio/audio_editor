//
//  ae_app_modal_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_action_receiver_providable.h>
#include <audio_editor_core/ae_app_modal_sub_lifetime.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct app_modal_lifecycle final : action_receiver_providable {
    [[nodiscard]] static std::shared_ptr<app_modal_lifecycle> make_shared();

    app_modal_lifecycle();

    [[nodiscard]] std::optional<app_modal_sub_lifetime> const &current() const;

    void add_project_setup_dialog();
    void remove_project_setup_dialog();
    [[nodiscard]] std::shared_ptr<project_setup_dialog_lifetime> const &project_setup_dialog_lifetime() const;

    [[nodiscard]] observing::syncable observe(std::function<void(std::optional<app_modal_sub_lifetime> const &)> &&);

   private:
    observing::value::holder_ptr<std::optional<app_modal_sub_lifetime>> const _current;

#pragma mark - action_receiver_provider

    std::optional<action_id> receivable_id() const override;
    std::vector<action_receivable *> receivers() const override;
    std::vector<action_receiver_providable *> sub_providers() const override;
};
}  // namespace yas::ae