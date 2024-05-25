//
//  nudging_dependencies.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <ae-core/project_editing/value_types/timing_types.h>

#include <observing/umbrella.hpp>

namespace yas::ae {
struct timing_for_nudging {
    virtual ~timing_for_nudging() = default;

    [[nodiscard]] virtual sample_rate_t sample_rate() const = 0;

    [[nodiscard]] virtual timing_components components(frame_index_t const) const = 0;
    [[nodiscard]] virtual timing_components floored_components(timing_unit_kind const, frame_index_t const) const = 0;
    [[nodiscard]] virtual frame_index_t frame(timing_components const &) const = 0;
};

struct app_settings_for_nudging {
    virtual ~app_settings_for_nudging() = default;

    virtual void set_timing_unit_kind(timing_unit_kind const) = 0;
    [[nodiscard]] virtual timing_unit_kind timing_unit_kind() const = 0;
    [[nodiscard]] virtual observing::syncable observe_timing_unit_kind(
        std::function<void(ae::timing_unit_kind const &)> &&) = 0;
};
}  // namespace yas::ae
