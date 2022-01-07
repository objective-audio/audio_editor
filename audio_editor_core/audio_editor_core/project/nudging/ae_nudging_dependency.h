//
//  ae_nudging_dependency.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

namespace yas::ae {
struct timing_for_nudging {
    virtual ~timing_for_nudging() = default;

    [[nodiscard]] virtual sample_rate_t sample_rate() const = 0;
    [[nodiscard]] virtual uint32_t unit_sample_count() const = 0;
};
}  // namespace yas::ae
