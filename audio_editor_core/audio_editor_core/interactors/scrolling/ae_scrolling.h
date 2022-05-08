//
//  ae_scrolling.h
//

#pragma once

#include <audio_editor_core/ae_player_dependency.h>
#include <audio_editor_core/ae_scroll_gesture_controller_dependency.h>
#include <audio_editor_core/ae_scrolling_types.h>

namespace yas::ae {
struct scrolling final : scrolling_for_player, scrolling_for_gesture_controller {
    [[nodiscard]] static std::shared_ptr<scrolling> make_shared();

    scrolling();

    void begin() override;
    void set_delta_time(double const) override;
    void end() override;
    [[nodiscard]] bool is_began() const override;

    void set_is_enabled(bool const) override;
    bool is_enabled() const;

    [[nodiscard]] observing::endable observe(std::function<void(scrolling_event const &)> &&) override;

   private:
    observing::notifier_ptr<scrolling_event> const _notifier;

    bool _is_enabled = true;
    bool _is_began = false;
};
}  // namespace yas::ae
