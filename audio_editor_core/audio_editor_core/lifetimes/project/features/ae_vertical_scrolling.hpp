//
//  ae_vertical_scrolling.hpp
//

#pragma once

namespace yas::ae {
struct vertical_scrolling final {
    vertical_scrolling();

    [[nodiscard]] double track() const;
    [[nodiscard]] observing::syncable observe_track(std::function<void(double const &)> &&);

    void begin();
    void set_delta_track(double const);
    void end();
    [[nodiscard]] bool is_began() const;

   private:
    observing::value::holder_ptr<double> const _track;

    bool _is_began = false;

    vertical_scrolling(vertical_scrolling const &) = delete;
    vertical_scrolling(vertical_scrolling &&) = delete;
    vertical_scrolling &operator=(vertical_scrolling const &) = delete;
    vertical_scrolling &operator=(vertical_scrolling &&) = delete;
};
}  // namespace yas::ae
