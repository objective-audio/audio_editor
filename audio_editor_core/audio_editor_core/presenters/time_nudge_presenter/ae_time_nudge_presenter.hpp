//
//  ae_time_nudge_presenter.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class nudge_settings;

struct time_nudge_presenter final {
    [[nodiscard]] static std::shared_ptr<time_nudge_presenter> make_shared(window_lifetime_id const &);

    time_nudge_presenter(std::shared_ptr<nudge_settings> const &);

    [[nodiscard]] std::optional<std::size_t> nudging_unit_index() const;
    [[nodiscard]] observing::syncable observe_nudging_unit_index(std::function<void(std::size_t const &)> &&);

   private:
    std::weak_ptr<nudge_settings> const _nudge_settings;
};
}  // namespace yas::ae
