//
//  selected_module.cpp
//

#include "selected_module.hpp"

using namespace yas;
using namespace yas::ae;

selected_module::selected_module(time::range const &range, track_index_t const track) : range(range), track(track) {
}

selected_module_object::selected_module_object(object_id const &identifier, selected_module const &value)
    : identifier(identifier), value(value) {
}

module_index selected_module_object::index() const {
    return {this->identifier, this->value.range, this->value.track};
}

std::string yas::to_string(ae::selected_module const &module) {
    return "{range:" + to_string(module.range) + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::selected_module const &value) {
    os << to_string(value);
    return os;
}
