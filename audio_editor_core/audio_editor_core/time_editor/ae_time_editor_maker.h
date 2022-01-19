//
//  ae_time_editor_maker.h
//

#pragma once

#include <audio_editor_core/ae_project_editor_dependency.h>

namespace yas::ae {
struct time_editor_maker final : time_editor_maker_for_project_editor {
    [[nodiscard]] static std::shared_ptr<time_editor_maker> make_shared();

    [[nodiscard]] std::shared_ptr<time_editor_for_project_editor> make() const override;

   private:
    time_editor_maker();
};
}  // namespace yas::ae
