//
//  ae_project_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_action_receiver_providable.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_sub_lifetime.h>
#include <observing/yas_observing_umbrella.h>

#include <memory>

namespace yas::ae {
struct project_lifecycle final : action_receiver_providable {
    [[nodiscard]] static std::shared_ptr<project_lifecycle> make_shared(project_id const &);

    project_lifecycle(project_id const &);

    ae::project_id const project_id;

    [[nodiscard]] std::optional<project_sub_lifetime> const &current() const;

    void switch_to_project();

    [[nodiscard]] observing::syncable observe(std::function<void(std::optional<project_sub_lifetime> const &)> &&);

   private:
    observing::value::holder_ptr<std::optional<project_sub_lifetime>> const _current;

#pragma mark - action_receiver_provider

    std::optional<action_id> receivable_id() const override;
    std::vector<action_receivable *> receivers() const override;
    std::vector<action_receiver_providable *> sub_providers() const override;
};
}  // namespace yas::ae
