//
//  ae_zooming.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct zooming {
    static std::shared_ptr<zooming> make_shared();

    void begin();
    void set_magnification(double const);
    void end();

    double scale() const;

    observing::syncable observe_scale(std::function<void(double const &)> &&);

   private:
    observing::value::holder_ptr<double> const _scale;

    std::optional<double> _began_scale = std::nullopt;
    std::optional<double> _magnification = std::nullopt;

    zooming();
};
}  // namespace yas::ae
