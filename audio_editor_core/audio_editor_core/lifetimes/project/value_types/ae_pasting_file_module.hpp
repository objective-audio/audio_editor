//
//  ae_pasting_file_module.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

#include <string>

namespace yas::ae {
struct pasting_file_module final {
    std::string name;
    frame_index_t file_frame;
    time::range range;
    std::string file_name;

    pasting_file_module(std::string const &name, frame_index_t const file_frame, time::range const &range,
                        std::string const &file_name);
};

struct pasting_file_module_object final {
    identifier identifier;
    pasting_file_module value;

    pasting_file_module_object(yas::identifier const &, pasting_file_module &&);
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::pasting_file_module const &);
}  // namespace yas

std::ostream &operator<<(std::ostream &os, yas::ae::pasting_file_module const &);
