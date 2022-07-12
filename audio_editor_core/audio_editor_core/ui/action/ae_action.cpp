//
//  ae_action.cpp
//

#include "ae_action.h"

using namespace yas;
using namespace yas::ae;

action::action(action_name const name, std::string const &value) : name(name), value(value) {
}

ae::action_name_kind yas::to_kind(ae::action_name const &scope) {
    using kind = ae::action_name_kind;

    if (std::holds_alternative<ae::editing_action_name>(scope)) {
        return kind::editing;
    } else if (std::holds_alternative<ae::time_editing_action_name>(scope)) {
        return kind::time_editing;
    } else {
        throw std::invalid_argument("invalid scope");
    }
}
