//
//  ae_file_module_utils.h
//

#pragma once

#include <audio_editor_core/ae_file_track_types.h>
#include <processing/yas_processing_time.h>

namespace yas::ae::file_module_utils {
std::optional<file_module> module(file_track_module_map_t const &, proc::frame_index_t const);
std::optional<file_module> first_module(file_track_module_map_t const &);
std::optional<file_module> last_module(file_track_module_map_t const &);
std::optional<file_module> previous_module(file_track_module_map_t const &, proc::frame_index_t const);
std::optional<file_module> next_module(file_track_module_map_t const &, proc::frame_index_t const);

bool can_split_time_range(proc::time::range const &, proc::frame_index_t const);
std::optional<file_module> splittable_module(file_track_module_map_t const &modules, proc::frame_index_t const frame);

std::vector<file_module> overlapped_modules(file_track_module_map_t const &, proc::time::range const &);
}  // namespace yas::ae::file_module_utils
