//
//  ae_track_selector_dependencies.h
//

#pragma once

#include <ae-core/global/value_types/ae_common_types.h>

#include <observing/umbrella.hpp>

namespace yas::ae {
struct scrolling_for_track_selector {
    virtual ~scrolling_for_track_selector() = default;

    [[nodiscard]] virtual track_index_t track() const = 0;
    [[nodiscard]] virtual observing::syncable observe_track(std::function<void(track_index_t const &)> &&) = 0;
};
}  // namespace yas::ae
