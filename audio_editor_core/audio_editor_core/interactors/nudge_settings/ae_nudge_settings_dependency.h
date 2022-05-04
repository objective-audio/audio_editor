//
//  ae_nudge_settings_dependency.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_timing_types.h>

namespace yas::ae {
struct timing_for_nudge_settings {
    virtual ~timing_for_nudge_settings() = default;

    [[nodiscard]] virtual sample_rate_t sample_rate() const = 0;

    [[nodiscard]] virtual timing_components components(frame_index_t const) const = 0;
    [[nodiscard]] virtual frame_index_t frame(timing_components const &) const = 0;
};
}  // namespace yas::ae
