//
//  ae_file_module.h
//

#pragma once

#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_time.h>

namespace yas::ae {
struct file_module {
    proc::time::range range;
    proc::frame_index_t file_frame;

    bool operator==(file_module const &rhs) const {
        return this->range == rhs.range && this->file_frame == rhs.file_frame;
    }

    bool operator!=(file_module const &rhs) const {
        return !(*this == rhs);
    }
};
}  // namespace yas::ae
