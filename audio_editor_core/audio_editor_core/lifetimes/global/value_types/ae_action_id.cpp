//
//  ae_action_id.cpp
//

#include "ae_action_id.h"

using namespace yas;
using namespace yas::ae;

action_id::action_id(window_lifetime_id const &lifetime_id)
    : window_instance_id(lifetime_id.instance_id), project_id(lifetime_id.project_id) {
}

bool yas::is_match(std::optional<ae::action_id> const &lhs, std::optional<ae::action_id> const &rhs) {
    if (!lhs.has_value() || !rhs.has_value()) {
        return true;
    }

    auto const &lhs_window_id = lhs.value().window_instance_id;
    auto const &rhs_window_id = rhs.value().window_instance_id;
    if (lhs_window_id.has_value() && rhs_window_id.has_value() && lhs_window_id != rhs_window_id) {
        return false;
    }

    auto const &lhs_project_id = lhs.value().project_id;
    auto const &rhs_project_id = rhs.value().project_id;
    if (lhs_project_id.has_value() && rhs_project_id.has_value() && lhs_project_id != rhs_project_id) {
        return false;
    }

    return true;
}
