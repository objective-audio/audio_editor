//
//  ae_action.cpp
//

#include "ae_action.h"

using namespace yas;
using namespace yas::ae;

action::action(action_name const name, std::optional<action_value> &&value) : name(name), value(std::move(value)) {
}

int64_t const &action::integer_value() const {
    return std::get<int64_t>(this->value.value());
}

std::string const &action::string_value() const {
    return std::get<std::string>(this->value.value());
}

time::range const &action::range_value() const {
    return std::get<time::range>(this->value.value());
}

module_index const &action::module_index_value() const {
    return std::get<module_index>(this->value.value());
}

marker_index const &action::marker_index_value() const {
    return std::get<marker_index>(this->value.value());
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
