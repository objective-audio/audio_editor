//
//  ae_time_editor_level.h
//

#pragma once

#include <memory>

namespace yas::ae {
class number_components;
class time_editor;

struct time_editor_level final {
    [[nodiscard]] static std::shared_ptr<time_editor_level> make_shared(number_components const &);

    std::shared_ptr<time_editor> const time_editor;

   private:
    time_editor_level(number_components const &);
};
}  // namespace yas::ae
