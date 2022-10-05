//
//  ae_pasting_file_module.cpp
//

#include "ae_pasting_file_module.hpp"

using namespace yas;
using namespace yas::ae;

pasting_file_module::pasting_file_module(std::string const &name, frame_index_t const file_frame,
                                         time::range const &range, std::string const &file_name)
    : name(name), file_frame(file_frame), range(range), file_name(file_name) {
}

pasting_file_module_object::pasting_file_module_object(yas::identifier const &identifier, pasting_file_module &&value)
    : identifier(identifier), value(std::move(value)) {
}

std::string yas::to_string(ae::pasting_file_module const &module) {
    return "{name:" + module.name + ", file_frame:" + std::to_string(module.file_frame) +
           ", range:" + to_string(module.range) + ", file_name:" + module.file_name + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::pasting_file_module const &value) {
    os << to_string(value);
    return os;
}
