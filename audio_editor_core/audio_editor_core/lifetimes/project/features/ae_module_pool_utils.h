//
//  ae_module_pool_utils.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_module_pool_types.h>

namespace yas::ae::module_pool_utils {
[[nodiscard]] std::optional<time::range> total_range(module_pool_module_map_t const &);
[[nodiscard]] std::optional<module_object> module(module_pool_module_map_t const &, frame_index_t const);
[[nodiscard]] std::optional<module_object> first_module(module_pool_module_map_t const &);
[[nodiscard]] std::optional<module_object> last_module(module_pool_module_map_t const &);
[[nodiscard]] std::optional<module_object> previous_module(module_pool_module_map_t const &, frame_index_t const);
[[nodiscard]] std::optional<module_object> next_module(module_pool_module_map_t const &, frame_index_t const);

[[nodiscard]] std::optional<module_object> splittable_module(module_pool_module_map_t const &modules,
                                                             frame_index_t const frame);

[[nodiscard]] std::vector<module_object> overlapped_modules(module_pool_module_map_t const &, time::range const &);
}  // namespace yas::ae::module_pool_utils
