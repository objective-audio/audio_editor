//
//  ae_db_types.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_db_edge.h>
#include <audio_editor_core/ae_db_marker.h>
#include <audio_editor_core/ae_db_module.h>

namespace yas::ae {
using db_modules_map = std::unordered_map<uintptr_t, db_module>;
using db_markers_map = std::unordered_map<uintptr_t, db_marker>;
}  // namespace yas::ae
