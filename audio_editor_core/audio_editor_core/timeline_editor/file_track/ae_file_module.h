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
};
}  // namespace yas::ae
