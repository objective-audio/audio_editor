//
//  ae_time_editor_level.h
//

#pragma once

#include <memory>

namespace yas::ae {
class time_editor;

struct time_editor_level final {
    [[nodiscard]] static std::shared_ptr<time_editor_level> make_shared(std::shared_ptr<time_editor> const &);

    std::shared_ptr<time_editor> const time_editor;

   private:
    time_editor_level(std::shared_ptr<ae::time_editor> const &);
};
}  // namespace yas::ae
