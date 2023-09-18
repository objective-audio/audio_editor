//
//  ae_module_pool_utils.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_module_pool_types.h>

#include <set>

namespace yas::ae::module_pool_utils {
[[nodiscard]] std::optional<time::range> total_range(module_pool_module_map_t const &);
[[nodiscard]] module_pool_module_map_t modules(module_pool_module_map_t const &, std::set<track_index_t> const &,
                                               frame_index_t const);
[[nodiscard]] bool has_modules(module_pool_module_map_t const &, std::set<track_index_t> const &, frame_index_t const);

[[nodiscard]] std::optional<frame_index_t> first_frame(module_pool_module_map_t const &);
[[nodiscard]] std::optional<frame_index_t> last_next_frame(module_pool_module_map_t const &);

[[nodiscard]] std::optional<frame_index_t> previous_jumpable_frame(module_pool_module_map_t const &,
                                                                   frame_index_t const);
[[nodiscard]] std::optional<frame_index_t> next_jumpable_frame(module_pool_module_map_t const &, frame_index_t const);

[[nodiscard]] module_pool_module_map_t splittable_modules(module_pool_module_map_t const &,
                                                          std::set<track_index_t> const &, frame_index_t const);
[[nodiscard]] bool has_splittable_modules(module_pool_module_map_t const &, std::set<track_index_t> const &,
                                          frame_index_t const);
[[nodiscard]] bool has_splittable_modules(std::set<module_index> const &, std::set<track_index_t> const &,
                                          frame_index_t const);

[[nodiscard]] std::vector<module_object> overlapped_modules(module_pool_module_map_t const &, track_index_t const,
                                                            time::range const &);
}  // namespace yas::ae::module_pool_utils
