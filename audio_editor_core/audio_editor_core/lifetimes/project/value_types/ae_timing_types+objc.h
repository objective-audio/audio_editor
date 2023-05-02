//
//  ae_timing_types+objc.h
//

#pragma once

#include <audio_editor_core/ae_timing_types.h>
#include <audio_editor_core/ae_timing_types_objc.h>

namespace yas {
ae::timing_fraction_kind to_cpp(TimingFractionKind const);
TimingFractionKind to_objc(ae::timing_fraction_kind const);

ae::timing_unit_kind to_cpp(TimingUnitKind const);
TimingUnitKind to_objc(ae::timing_unit_kind const);
}  // namespace yas
