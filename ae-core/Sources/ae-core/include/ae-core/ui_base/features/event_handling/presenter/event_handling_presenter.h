//
//  event_handling_presenter.h
//

#pragma once

#include <ae-core/global/value_types/action.h>
#include <ae-core/global/value_types/project_lifetime_id.h>

namespace yas::ae {
class player;
class action_sender;

struct event_handling_presenter final {
    [[nodiscard]] static std::shared_ptr<event_handling_presenter> make_shared(project_lifetime_id const &);

    event_handling_presenter(project_lifetime_id const &, std::shared_ptr<action_sender> const &);

    [[nodiscard]] bool responds_to_action(action const);

   private:
    project_lifetime_id const _project_lifetime_id;
    std::weak_ptr<action_sender> const _action_sender;

    event_handling_presenter(event_handling_presenter const &) = delete;
    event_handling_presenter(event_handling_presenter &&) = delete;
    event_handling_presenter &operator=(event_handling_presenter const &) = delete;
    event_handling_presenter &operator=(event_handling_presenter &&) = delete;
};
}  // namespace yas::ae
