//
//  ae_file_module.h
//

#pragma once

#include <audio_editor_core/ae_file_module_index.hpp>
#include <string>

namespace yas::ae {
struct file_module final {
    std::string name;
    time::range range;
    frame_index_t file_frame;
    std::string file_name;

    file_module(std::string const &name, time::range const &, frame_index_t const file_frame,
                std::string const &file_name);

    [[nodiscard]] std::optional<file_module> head_dropped(frame_index_t const) const;
    [[nodiscard]] std::optional<file_module> tail_dropped(frame_index_t const) const;
    [[nodiscard]] file_module offset(frame_index_t const) const;
};

struct file_module_object final {
    object_id identifier;
    file_module value;

    file_module_object(object_id const &, file_module &&);

    [[nodiscard]] file_module_index index() const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::file_module const &);
}

std::ostream &operator<<(std::ostream &os, yas::ae::file_module const &value);
