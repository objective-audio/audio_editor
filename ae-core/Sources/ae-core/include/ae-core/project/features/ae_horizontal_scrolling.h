//
//  ae_horizontal_scrolling.h
//

#pragma once

#include <ae-core/project/features/ae_player_dependency.h>
#include <ae-core/project/value_types/ae_scrolling_types.h>
#include <ae-core/project_editing/features/ae_scroll_gesture_controller_dependency.h>

namespace yas::ae {
struct horizontal_scrolling final : scrolling_for_player, horizontal_scrolling_for_gesture_controller {
    horizontal_scrolling();

    void begin() override;
    void set_delta_time(double const) override;
    void end() override;
    [[nodiscard]] bool is_began() const override;

    void set_is_enabled(bool const) override;
    [[nodiscard]] bool is_enabled() const;

    [[nodiscard]] observing::endable observe(std::function<void(scrolling_event const &)> &&) override;

   private:
    observing::notifier_ptr<scrolling_event> const _notifier;

    bool _is_enabled = true;
    bool _is_began = false;

    horizontal_scrolling(horizontal_scrolling const &) = delete;
    horizontal_scrolling(horizontal_scrolling &&) = delete;
    horizontal_scrolling &operator=(horizontal_scrolling const &) = delete;
    horizontal_scrolling &operator=(horizontal_scrolling &&) = delete;
};
}  // namespace yas::ae
