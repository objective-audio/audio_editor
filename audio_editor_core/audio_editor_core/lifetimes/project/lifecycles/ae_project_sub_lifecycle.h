//
//  ae_project_sub_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_action_receiver_providable.h>
#include <audio_editor_core/ae_project_lifetime_id.h>
#include <audio_editor_core/ae_project_sub_lifetime.h>

#include <memory>
#include <observing/yas_observing_umbrella.hpp>

namespace yas::ae {
struct project_sub_lifecycle final : action_receiver_providable {
    project_sub_lifecycle(project_lifetime_id const &);

    ae::project_lifetime_id const project_lifetime_id;

    [[nodiscard]] std::optional<project_sub_lifetime> const &current() const;

    void switch_to_project_launch();
    void switch_to_project();

    [[nodiscard]] observing::syncable observe(std::function<void(std::optional<project_sub_lifetime> const &)> &&);

   private:
    observing::value::holder_ptr<std::optional<project_sub_lifetime>> const _current;

    project_sub_lifecycle(project_sub_lifecycle const &) = delete;
    project_sub_lifecycle(project_sub_lifecycle &&) = delete;
    project_sub_lifecycle &operator=(project_sub_lifecycle const &) = delete;
    project_sub_lifecycle &operator=(project_sub_lifecycle &&) = delete;

#pragma mark - action_receiver_provider

    std::optional<action_id> receivable_id() const override;
    std::vector<action_receivable *> receivers() const override;
    std::vector<action_receiver_providable *> sub_providers() const override;
};
}  // namespace yas::ae
