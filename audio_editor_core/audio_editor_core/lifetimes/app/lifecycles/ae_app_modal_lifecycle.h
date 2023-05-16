//
//  ae_app_modal_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_action_receiver_providable.h>
#include <audio_editor_core/ae_app_modal_sub_lifetime.h>
#include <audio_editor_core/ae_app_sub_lifetime_id.h>

#include <observing/yas_observing_umbrella.hpp>

namespace yas::ae {
class id_generatable;

struct app_modal_lifecycle final : action_receiver_providable {
    app_modal_lifecycle(id_generatable const *);

    [[nodiscard]] std::optional<app_modal_sub_lifetime> const &current() const;

    void add_project_setup_dialog();
    void remove_project_setup_dialog(project_setup_dialog_lifetime_id const &);
    [[nodiscard]] std::shared_ptr<project_setup_dialog_lifetime> const &project_setup_dialog_lifetime() const;

    [[nodiscard]] observing::syncable observe(std::function<void(std::optional<app_modal_sub_lifetime> const &)> &&);

   private:
    id_generatable const *const _id_generator;
    observing::value::holder_ptr<std::optional<app_modal_sub_lifetime>> const _current;

    app_modal_lifecycle(app_modal_lifecycle const &) = delete;
    app_modal_lifecycle(app_modal_lifecycle &&) = delete;
    app_modal_lifecycle &operator=(app_modal_lifecycle const &) = delete;
    app_modal_lifecycle &operator=(app_modal_lifecycle &&) = delete;

#pragma mark - action_receiver_provider

    std::optional<action_id> receivable_id() const override;
    std::vector<action_receivable *> receivers() const override;
    std::vector<action_receiver_providable *> sub_providers() const override;
};
}  // namespace yas::ae
