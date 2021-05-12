//
//  ae_file_module_utils.h
//

#pragma once

#include <processing/yas_processing_time.h>

namespace yas::ae::file_module_utils {
bool can_split_time_range(proc::time::range const &, proc::frame_index_t const);
}
