//
//  ae_action.cpp
//

#include "ae_action.h"

using namespace yas;
using namespace yas::ae;

action::action(action_kind const kind, std::string const &value, std::optional<ae::action_id> const &action_id)
    : kind(kind), value(value), action_id(action_id) {
}
