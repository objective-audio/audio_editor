//
//  ae_zooming_pair.h
//

#pragma once

#include <audio_editor_core/ae_zooming.h>
#include <audio_editor_core/ae_zooming_types.h>

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
};
}  // namespace yas::ae
