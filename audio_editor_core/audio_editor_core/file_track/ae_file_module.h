//
//  ae_file_module.h
//

#pragma once

#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_time.h>

#include <string>

namespace yas::ae {
struct file_module {
    proc::time::range range;
    proc::frame_index_t file_frame;

    bool operator==(file_module const &rhs) const;
    bool operator!=(file_module const &rhs) const;

    std::optional<file_module> head_dropped(proc::frame_index_t const) const;
    std::optional<file_module> tail_dropped(proc::frame_index_t const) const;
    file_module offset(proc::frame_index_t const) const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::file_module const &);
}
