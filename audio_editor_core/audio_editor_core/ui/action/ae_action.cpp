//
//  ae_action.cpp
//

#include "ae_action.h"

using namespace yas;
using namespace yas::ae;

action::action(action_kind const kind, std::optional<ae::action_id> const &action_id, std::string const &value)
    : kind(kind), action_id(action_id), value(value) {
}
