//
//  ae_file_track_utils.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_file_track_types.h>

namespace yas::ae::file_track_utils {
[[nodiscard]] std::optional<time::range> total_range(file_track_module_map_t const &);
[[nodiscard]] std::optional<file_module_object> module(file_track_module_map_t const &, frame_index_t const);
[[nodiscard]] std::optional<file_module_object> first_module(file_track_module_map_t const &);
[[nodiscard]] std::optional<file_module_object> last_module(file_track_module_map_t const &);
[[nodiscard]] std::optional<file_module_object> previous_module(file_track_module_map_t const &, frame_index_t const);
[[nodiscard]] std::optional<file_module_object> next_module(file_track_module_map_t const &, frame_index_t const);

[[nodiscard]] std::optional<file_module_object> splittable_module(file_track_module_map_t const &modules,
                                                                  frame_index_t const frame);

[[nodiscard]] std::vector<file_module_object> overlapped_modules(file_track_module_map_t const &, time::range const &);
}  // namespace yas::ae::file_track_utils
