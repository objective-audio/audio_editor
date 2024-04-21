//
//  ae_action_id.cpp
//

#include "ae_action_id.h"

using namespace yas;
using namespace yas::ae;

action_id::action_id(project_lifetime_id const &lifetime_id)
    : project_instance(lifetime_id.instance), project_guid(lifetime_id.guid) {
}

bool yas::is_match(std::optional<ae::action_id> const &lhs, std::optional<ae::action_id> const &rhs) {
    if (!lhs.has_value() || !rhs.has_value()) {
        return true;
    }

    auto const &lhs_instance_id = lhs.value().project_instance;
    auto const &rhs_instance_id = rhs.value().project_instance;
    if (lhs_instance_id.has_value() && rhs_instance_id.has_value() && lhs_instance_id != rhs_instance_id) {
        return false;
    }

    auto const &lhs_guid = lhs.value().project_guid;
    auto const &rhs_guid = rhs.value().project_guid;
    if (lhs_guid.has_value() && rhs_guid.has_value() && lhs_guid != rhs_guid) {
        return false;
    }

    return true;
}
