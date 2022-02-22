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

bool action::operator==(action const &rhs) const {
    return this->kind == rhs.kind && this->value == rhs.value;
}

bool action::operator!=(action const &rhs) const {
    return !(*this == rhs);
}
