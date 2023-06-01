//
//  ae_vertical_scrolling.hpp
//

#pragma once

namespace yas::ae {
struct vertical_scrolling final {
    vertical_scrolling();

    [[nodiscard]] double track() const;

    void begin();
    void set_delta_track(double const);
    void end();
    [[nodiscard]] bool is_began() const;

   private:
    double _track;

    bool _is_began = false;

    vertical_scrolling(vertical_scrolling const &) = delete;
    vertical_scrolling(vertical_scrolling &&) = delete;
    vertical_scrolling &operator=(vertical_scrolling const &) = delete;
    vertical_scrolling &operator=(vertical_scrolling &&) = delete;
};
}  // namespace yas::ae
