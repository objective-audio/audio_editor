//
//  timing_utils.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <ae-core/project_editing/value_types/timing_types.h>

namespace yas::ae::timing_utils {
[[nodiscard]] uint32_t to_fraction_unit(timing_fraction_kind const, sample_rate_t const);
[[nodiscard]] uint32_t to_fraction_digits(timing_fraction_kind const, sample_rate_t const);
[[nodiscard]] uint32_t to_fraction_value(frame_index_t const, timing_fraction_kind const, sample_rate_t const);
[[nodiscard]] timing_components to_components(frame_index_t const, timing_fraction_kind const, sample_rate_t const);
[[nodiscard]] timing_components to_floored_components(frame_index_t const, timing_unit_kind const,
                                                      timing_fraction_kind const, sample_rate_t const);
[[nodiscard]] frame_index_t to_frame(timing_components const &, timing_fraction_kind const, sample_rate_t const);
}  // namespace yas::ae::timing_utils
