//
//  ae_time_editor.h
//

#pragma once

#include <audio_editor_core/ae_project_editor_dependency.h>

namespace yas::ae {
struct time_editor final : time_editor_for_project_editor {
    [[nodiscard]] static std::shared_ptr<time_editor> make_shared();

    void input_number(uint32_t const) override;

   private:
    time_editor();
};
}  // namespace yas::ae
