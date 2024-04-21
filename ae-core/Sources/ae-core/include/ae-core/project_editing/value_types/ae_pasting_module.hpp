//
//  ae_pasting_module.hpp
//

#pragma once

#include <ae-core/project_editing/value_types/ae_pasting_module_index.hpp>
#include <string>

namespace yas::ae {
struct pasting_module final {
    std::string name;
    frame_index_t file_frame;
    time::range range;
    track_index_t track;
    std::string file_name;

    pasting_module(std::string const &name, frame_index_t const file_frame, time::range const &range,
                   track_index_t const track, std::string const &file_name);
};

struct pasting_module_object final {
    identifier identifier;
    pasting_module value;

    pasting_module_object(yas::identifier const &, pasting_module &&);

    pasting_module_index index() const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::pasting_module const &);
}  // namespace yas

std::ostream &operator<<(std::ostream &os, yas::ae::pasting_module const &);
