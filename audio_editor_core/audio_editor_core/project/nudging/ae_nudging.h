//
//  ae_nudging.h
//

#pragma once

#include <audio_editor_core/ae_nudging_types.h>

#include <memory>

namespace yas::ae {
struct nudging final {
    [[nodiscard]] static std::shared_ptr<nudging> make_shared();

   private:
    nudging();
};
}  // namespace yas::ae
