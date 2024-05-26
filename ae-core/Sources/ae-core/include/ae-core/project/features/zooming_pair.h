//
//  zooming_pair.h
//

#pragma once

#include <ae-core/project/features/zooming.h>

#include <ae-core/project/value_types/zooming_types.hpp>

namespace yas::ae {
struct zooming_pair final {
    zooming_pair();

    std::shared_ptr<zooming> const horizontal;
    std::shared_ptr<zooming> const vertical;

    [[nodiscard]] zooming_scale scale() const;
    [[nodiscard]] observing::syncable observe_scale(std::function<void(zooming_scale const &)> &&);

   private:
    observing::fetcher_ptr<zooming_scale> _scale_fetcher;
    observing::canceller_pool _pool;

    zooming_pair(zooming_pair const &) = delete;
    zooming_pair(zooming_pair &&) = delete;
    zooming_pair &operator=(zooming_pair const &) = delete;
    zooming_pair &operator=(zooming_pair &&) = delete;
};
}  // namespace yas::ae
