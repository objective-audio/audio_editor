//
//  ae_range_selector_dependency.h
//

#pragma once

#include <observing/yas_observing_umbrella.hpp>

namespace yas::ae {
struct player_for_range_selector {
    virtual ~player_for_range_selector() = default;

    [[nodiscard]] virtual bool is_playing() const = 0;
    [[nodiscard]] virtual observing::syncable observe_is_playing(std::function<void(bool const &)> &&) = 0;
};

struct deselector_for_range_selector {
    virtual ~deselector_for_range_selector() = default;

    virtual void deselect_all() = 0;
};
}  // namespace yas::ae
