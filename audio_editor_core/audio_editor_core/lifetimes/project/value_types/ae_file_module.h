//
//  ae_file_module.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

#include <string>

namespace yas::ae {
struct file_module_object final {
    struct params {
        std::string name;
        time::range range;
        frame_index_t file_frame;
        std::string file_name;

        params(std::string const &name, time::range const &, frame_index_t const file_frame,
               std::string const &file_name);
    };

    object_id identifier;
    std::string name;
    time::range range;
    frame_index_t file_frame;
    std::string file_name;

    file_module_object(object_id const &, params &&);
    file_module_object(object_id const &, std::string const &name, time::range const &range,
                       frame_index_t const file_frame, std::string const &file_name);

    [[nodiscard]] file_module_object::params parameters() const;
    [[nodiscard]] std::optional<file_module_object::params> head_dropped(frame_index_t const) const;
    [[nodiscard]] std::optional<file_module_object::params> tail_dropped(frame_index_t const) const;
    [[nodiscard]] file_module_object::params offset(frame_index_t const) const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::file_module_object const &);
}

std::ostream &operator<<(std::ostream &os, yas::ae::file_module_object const &value);
