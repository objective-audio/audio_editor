//
//  ae_time_nudge_presenter.hpp
//

#pragma once

#include <audio_editor_core/ae_timing_types.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class nudging;

struct time_nudge_presenter final {
    [[nodiscard]] static std::shared_ptr<time_nudge_presenter> make_shared(window_lifetime_id const &);

    time_nudge_presenter(std::shared_ptr<nudging> const &);

    [[nodiscard]] std::optional<std::size_t> nudging_unit_index() const;
    [[nodiscard]] observing::syncable observe_nudging_unit_kind(std::function<void(timing_unit_kind const &)> &&);

   private:
    std::weak_ptr<nudging> const _nudging;

    time_nudge_presenter(time_nudge_presenter const &) = delete;
    time_nudge_presenter(time_nudge_presenter &&) = delete;
    time_nudge_presenter &operator=(time_nudge_presenter const &) = delete;
    time_nudge_presenter &operator=(time_nudge_presenter &&) = delete;
};
}  // namespace yas::ae
