//
//  ae_time_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_timing_types.h>

namespace yas::ae {
struct timing_for_time_presenter {
    virtual ~timing_for_time_presenter() = default;

    [[nodiscard]] virtual sample_rate_t sample_rate() const = 0;

    [[nodiscard]] virtual timing_fraction_kind fraction_kind() const = 0;
    [[nodiscard]] virtual uint32_t fraction_digits() const = 0;
    [[nodiscard]] virtual uint32_t fraction_value(frame_index_t const frame) const = 0;
};

struct project_editor_for_time_presenter {
    virtual ~project_editor_for_time_presenter() = default;

    [[nodiscard]] virtual frame_index_t current_frame() const = 0;
};
}  // namespace yas::ae
