//
//  project_action_sender.h
//

#pragma once

#include <ae-core/global/value_types/action.h>
#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ae-core/ui_resource/features/key.h>

namespace yas::ae {
class action_sender;

struct project_action_sender final {
    project_action_sender(project_lifetime_id const &, action_sender *);

    void send(action const &);
    void send(action_name const &, std::optional<action_value> &&value = std::nullopt);
    void send(ae::key const);

   private:
    project_lifetime_id const _project_lifetime_id;
    action_sender *const _action_sender;

    project_action_sender(project_action_sender const &) = delete;
    project_action_sender(project_action_sender &&) = delete;
    project_action_sender &operator=(project_action_sender const &) = delete;
    project_action_sender &operator=(project_action_sender &&) = delete;
};
}  // namespace yas::ae
