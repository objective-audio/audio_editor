//
//  ae_file_module.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_identifier.h>

#include <string>

namespace yas::ae {
struct file_module {
    identifier identifier;
    time::range range;
    frame_index_t file_frame;

    bool is_equal_location(file_module const &rhs) const;

    std::optional<file_module> head_dropped(frame_index_t const) const;
    std::optional<file_module> tail_dropped(frame_index_t const) const;
    file_module offset(frame_index_t const) const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::file_module const &);
}

std::ostream &operator<<(std::ostream &os, yas::ae::file_module const &value);
