//
//  ae_app_lifecycle.h
//

#pragma once

#include <ae-core/global/interfaces/ae_action_receiver_providable.h>

#include <memory>

namespace yas::ae {
class app_lifetime;

struct app_lifecycle final : action_receiver_providable {
    app_lifecycle();

    std::shared_ptr<app_lifetime> const &lifetime() const;

    void add();

   private:
    std::shared_ptr<app_lifetime> _lifetime;

    app_lifecycle(app_lifecycle const &) = delete;
    app_lifecycle(app_lifecycle &&) = delete;
    app_lifecycle &operator=(app_lifecycle const &) = delete;
    app_lifecycle &operator=(app_lifecycle &&) = delete;

#pragma mark - action_receiver_provider

    std::optional<action_id> receivable_id() const override;
    std::vector<action_receivable *> receivers() const override;
    std::vector<action_receiver_providable *> sub_providers() const override;
};
}  // namespace yas::ae
