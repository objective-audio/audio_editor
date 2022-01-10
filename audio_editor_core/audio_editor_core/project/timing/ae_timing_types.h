//
//  ae_timing_types.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

namespace yas::ae {
enum class timing_fraction_kind {
    sample,
    milisecond,
    frame30,
};
}  // namespace yas::ae
