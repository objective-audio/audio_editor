//
//  module.h
//

#pragma once

#include <ae-core/project_editing/value_types/module_index.hpp>
#include <string>

namespace yas::ae {
struct module final {
    std::string name;
    time::range range;
    track_index_t track;
    frame_index_t file_frame;
    std::string file_name;

    module(std::string const &name, time::range const &, track_index_t const track, frame_index_t const file_frame,
           std::string const &file_name);

    [[nodiscard]] std::optional<module> head_dropped(frame_index_t const) const;
    [[nodiscard]] std::optional<module> tail_dropped(frame_index_t const) const;
    [[nodiscard]] module offset(frame_index_t const) const;
};

struct module_object final {
    object_id identifier;
    module value;

    module_object(object_id const &, module &&);

    [[nodiscard]] module_index index() const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::module const &);
}

std::ostream &operator<<(std::ostream &os, yas::ae::module const &value);
