//
//  ae_time_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_timing_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct timing_for_time_presenter {
    virtual ~timing_for_time_presenter() = default;

    [[nodiscard]] virtual sample_rate_t sample_rate() const = 0;

    [[nodiscard]] virtual timing_fraction_kind fraction_kind() const = 0;
    [[nodiscard]] virtual timing_components components(frame_index_t const) const = 0;
    [[nodiscard]] virtual uint32_t fraction_digits() const = 0;
};

struct time_editor_for_time_presenter {
    virtual ~time_editor_for_time_presenter() = default;

    [[nodiscard]] virtual std::size_t unit_index() const = 0;
    [[nodiscard]] virtual number_components editing_components() const = 0;

    [[nodiscard]] virtual observing::syncable observe_unit_index(std::function<void(std::size_t const &)> &&) = 0;
};

struct project_editor_for_time_presenter {
    virtual ~project_editor_for_time_presenter() = default;

    [[nodiscard]] virtual frame_index_t current_frame() const = 0;

    [[nodiscard]] virtual std::size_t nudging_unit_index() const = 0;

    [[nodiscard]] virtual observing::syncable observe_time_editor_for_time_presenter(
        std::function<void(std::shared_ptr<time_editor_for_time_presenter> const &)> &&) = 0;
    [[nodiscard]] virtual observing::syncable observe_nudging_unit_index(
        std::function<void(std::size_t const &)> &&) = 0;
};
}  // namespace yas::ae
