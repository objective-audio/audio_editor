//
//  ae_db_types.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_db_edge.h>
#include <audio_editor_core/ae_db_marker.h>
#include <audio_editor_core/ae_db_module.h>

#include <audio_editor_core/ae_db_file_ref.hpp>

namespace yas::ae {
using db_modules_map = std::unordered_map<object_id, db_module>;
using db_file_refs_map = std::unordered_map<std::string, db_file_ref>;
using db_markers_map = std::unordered_map<object_id, db_marker>;
}  // namespace yas::ae
