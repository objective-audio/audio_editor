//
//  ae_track_presenter_dependency.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct zooming_for_track_presenter {
    virtual ~zooming_for_track_presenter() = default;

    [[nodiscard]] virtual double scale() const = 0;

    [[nodiscard]] virtual observing::syncable observe_scale(std::function<void(double const &)> &&) = 0;
};
}  // namespace yas::ae
