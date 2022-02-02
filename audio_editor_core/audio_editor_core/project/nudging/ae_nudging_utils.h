//
//  ae_nudging_utils.h
//

#pragma once

#include <audio_editor_core/ae_nudging_dependency.h>
#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_timing_types.h>

#include <cstdint>

namespace yas::ae::nudging_utils {
timing_components offset_components(bool const is_previous, uint32_t const count, std::size_t const unit_idx,
                                    std::size_t const fraction_unit_size);
}  // namespace yas::ae::nudging_utils
