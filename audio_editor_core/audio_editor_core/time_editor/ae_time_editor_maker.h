//
//  ae_time_editor_maker.h
//

#pragma once

#include <audio_editor_core/ae_project_editor_dependency.h>

namespace yas::ae {
class time_editor;

struct time_editor_maker final {
    [[nodiscard]] static std::shared_ptr<time_editor_maker> make_shared();

    [[nodiscard]] std::shared_ptr<time_editor> make(number_components const &) const;

   private:
    time_editor_maker();
};
}  // namespace yas::ae
