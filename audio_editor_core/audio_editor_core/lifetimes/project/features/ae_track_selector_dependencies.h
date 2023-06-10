//
//  ae_track_selector_dependencies.h
//

#pragma once

#include <observing/yas_observing_umbrella.hpp>

namespace yas::ae {
struct scrolling_for_track_selector {
    virtual ~scrolling_for_track_selector() = default;

    [[nodiscard]] virtual observing::syncable observe_track(std::function<void(double const &)> &&) = 0;
};
}  // namespace yas::ae
