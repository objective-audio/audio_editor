//
//  ae_file_module_utils.h
//

#pragma once

#include <audio_editor_core/ae_file_track_types.h>
#include <processing/yas_processing_time.h>

namespace yas::ae {
class file_module;
}

namespace yas::ae::file_module_utils {
std::optional<file_module> module(file_track_module_map_t const &, proc::frame_index_t const);
std::optional<file_module> previous_module(file_track_module_map_t const &, proc::frame_index_t const);
std::optional<file_module> next_module(file_track_module_map_t const &, proc::frame_index_t const);

bool can_split_time_range(proc::time::range const &, proc::frame_index_t const);
}  // namespace yas::ae::file_module_utils
