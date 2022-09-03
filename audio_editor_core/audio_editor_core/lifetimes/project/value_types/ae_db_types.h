//
//  ae_db_types.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_db_edge.h>
#include <audio_editor_core/ae_db_marker.h>
#include <audio_editor_core/ae_db_module.h>

#include <audio_editor_core/ae_db_file_reference.hpp>

namespace yas::ae {
using db_modules_map = std::map<time::range, db_module>;
using db_file_refs_map = std::map<std::string, db_file_reference>;
using db_markers_map = std::map<frame_index_t, db_marker>;
}  // namespace yas::ae
