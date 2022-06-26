//
//  ae_action.cpp
//

#include "ae_action.h"

using namespace yas;
using namespace yas::ae;

action::action(action_kind const kind) : action(kind, "") {
}

action::action(action_kind const kind, std::string const &value) : kind(kind), value(value) {
}
