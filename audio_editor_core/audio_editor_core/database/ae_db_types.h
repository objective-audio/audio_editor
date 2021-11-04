//
//  ae_db_types.h
//

#pragma once

#include <audio_editor_core/ae_db_marker.h>
#include <audio_editor_core/ae_db_module.h>
#include <processing/yas_processing_time.h>

namespace yas::ae {
using db_modules_map = std::map<proc::time::range, db_module>;
using db_markers_map = std::map<proc::frame_index_t, db_marker>;
}  // namespace yas::ae
