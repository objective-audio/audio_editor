//
//  ae_nudging_utils.h
//

#pragma once

#include <audio_editor_core/ae_nudging_dependency.h>
#include <audio_editor_core/ae_nudging_types.h>

#include <cstdint>

namespace yas::ae::nudging_utils {
uint32_t to_sample_count(nudging_kind const, std::shared_ptr<timing_for_nudging> const &);
}
