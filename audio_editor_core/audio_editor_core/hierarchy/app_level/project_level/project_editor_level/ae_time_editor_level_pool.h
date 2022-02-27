//
//  ae_time_editor_level_pool.h
//

#pragma once

#include <memory>

namespace yas::ae {
struct time_editor_level_pool final {
    [[nodiscard]] static std::shared_ptr<time_editor_level_pool> make_shared();

   private:
    time_editor_level_pool();
};
}  // namespace yas::ae
