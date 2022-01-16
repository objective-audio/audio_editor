//
//  ae_nudging_utils.h
//

#pragma once

#include <audio_editor_core/ae_nudging_dependency.h>
#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_timing_types.h>

#include <cstdint>

namespace yas::ae::nudging_utils {
timing_components offset_components(bool const is_previous, uint32_t const offset, nudging_kind const,
                                    uint32_t const fraction_unit_count);
}  // namespace yas::ae::nudging_utils
