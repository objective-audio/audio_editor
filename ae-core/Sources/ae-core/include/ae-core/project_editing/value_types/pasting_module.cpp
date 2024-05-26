//
//  pasting_module.cpp
//

#include "pasting_module.hpp"

using namespace yas;
using namespace yas::ae;

pasting_module::pasting_module(std::string const &name, frame_index_t const file_frame, time::range const &range,
                               track_index_t const track, std::string const &file_name)
    : name(name), file_frame(file_frame), range(range), track(track), file_name(file_name) {
}

pasting_module_object::pasting_module_object(yas::identifier const &identifier, pasting_module &&value)
    : identifier(identifier), value(std::move(value)) {
}

pasting_module_index pasting_module_object::index() const {
    return {this->identifier, this->value.range, this->value.track};
}

std::string yas::to_string(ae::pasting_module const &module) {
    return "{name:" + module.name + ", file_frame:" + std::to_string(module.file_frame) +
           ", range:" + to_string(module.range) + ", track:" + std::to_string(module.track) +
           ", file_name:" + module.file_name + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::pasting_module const &value) {
    os << to_string(value);
    return os;
}
