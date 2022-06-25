//
//  ae_action_id.cpp
//

#include "ae_action_id.h"

bool yas::is_match(std::optional<ae::action_id> const &lhs, std::optional<ae::action_id> const &rhs) {
    if (!lhs.has_value() || !rhs.has_value()) {
        return true;
    }

    return lhs.value().project_id == rhs.value().project_id;
}
