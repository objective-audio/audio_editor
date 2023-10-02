//
//  ae_vertical_scrolling.hpp
//

#pragma once

#include <audio_editor_core/ae_track_selector_dependencies.h>

#include <observing/yas_observing_umbrella.hpp>

namespace yas::ae {
struct vertical_scrolling final : scrolling_for_track_selector {
    vertical_scrolling();

    [[nodiscard]] double track_position() const;
    [[nodiscard]] track_index_t track() const override;
    [[nodiscard]] double position_offset() const;
    [[nodiscard]] observing::syncable observe_track_position(std::function<void(double const &)> &&);
    [[nodiscard]] observing::syncable observe_track(std::function<void(track_index_t const &)> &&) override;

    void begin();
    void set_delta_position(double const);
    void end();
    [[nodiscard]] bool is_began() const;

    void move_to_previous_track(track_index_t const offset);
    void move_to_next_track(track_index_t const offset);

   private:
    observing::value::holder_ptr<double> const _position;

    bool _is_began = false;

    vertical_scrolling(vertical_scrolling const &) = delete;
    vertical_scrolling(vertical_scrolling &&) = delete;
    vertical_scrolling &operator=(vertical_scrolling const &) = delete;
    vertical_scrolling &operator=(vertical_scrolling &&) = delete;
};
}  // namespace yas::ae
