//
//  zooming.h
//

#pragma once

#include <observing/umbrella.hpp>

namespace yas::ae {
struct zooming final {
    zooming();

    void begin();
    void set_magnification(double const);
    void end();

    [[nodiscard]] double scale() const;

    [[nodiscard]] observing::syncable observe_scale(std::function<void(double const &)> &&);

   private:
    observing::value::holder_ptr<double> const _scale;

    std::optional<double> _began_scale = std::nullopt;
    std::optional<double> _magnification = std::nullopt;

    zooming(zooming const &) = delete;
    zooming(zooming &&) = delete;
    zooming &operator=(zooming const &) = delete;
    zooming &operator=(zooming &&) = delete;
};
}  // namespace yas::ae
