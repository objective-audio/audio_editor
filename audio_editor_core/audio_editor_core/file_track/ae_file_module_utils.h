//
//  ae_file_module_utils.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_file_track_types.h>

namespace yas::ae::file_module_utils {
[[nodiscard]] std::optional<file_module> module(file_track_module_map_t const &, frame_index_t const);
[[nodiscard]] std::optional<file_module> first_module(file_track_module_map_t const &);
[[nodiscard]] std::optional<file_module> last_module(file_track_module_map_t const &);
[[nodiscard]] std::optional<file_module> previous_module(file_track_module_map_t const &, frame_index_t const);
[[nodiscard]] std::optional<file_module> next_module(file_track_module_map_t const &, frame_index_t const);

[[nodiscard]] bool can_split_time_range(time::range const &, frame_index_t const);
[[nodiscard]] std::optional<file_module> splittable_module(file_track_module_map_t const &modules,
                                                           frame_index_t const frame);

[[nodiscard]] std::vector<file_module> overlapped_modules(file_track_module_map_t const &, time::range const &);
}  // namespace yas::ae::file_module_utils
