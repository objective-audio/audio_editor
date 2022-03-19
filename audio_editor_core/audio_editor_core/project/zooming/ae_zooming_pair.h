//
//  ae_zooming_pair.h
//

#pragma once

#include <audio_editor_core/ae_zooming.h>

namespace yas::ae {
struct zooming_pair final {
    [[nodiscard]] static std::shared_ptr<zooming_pair> make_shared();

    std::shared_ptr<zooming> const horizontal;
    std::shared_ptr<zooming> const vertical;

   private:
    zooming_pair();
};
}  // namespace yas::ae
