//
//  ae_app_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_action_receiver_providable.h>

#include <memory>

namespace yas::ae {
class app_lifetime;

struct app_lifecycle final : action_receiver_providable {
    [[nodiscard]] static std::shared_ptr<app_lifecycle> make_shared();

    app_lifecycle();

    std::shared_ptr<app_lifetime> const &lifetime() const;

    void add();

   private:
    std::shared_ptr<app_lifetime> _lifetime;

#pragma mark - action_receiver_provider

    std::optional<action_id> receivable_id() const override;
    std::vector<action_receivable *> receivers() const override;
    std::vector<action_receiver_providable *> sub_providers() const override;
};
}  // namespace yas::ae
