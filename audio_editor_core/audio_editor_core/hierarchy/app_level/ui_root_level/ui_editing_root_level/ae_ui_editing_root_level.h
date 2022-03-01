//
//  ae_ui_editing_root_level.h
//

#pragma once

#include <audio_editor_core/ae_ui_project_id.h>

#include <memory>

namespace yas::ae {
class ui_editing_root;

struct ui_editing_root_level final {
    [[nodiscard]] static std::shared_ptr<ui_editing_root_level> make_shared(ui_project_id const &project_id);

    std::shared_ptr<ui_editing_root> const ui_editing_root;

   private:
    ui_editing_root_level(ui_project_id const &project_id);
};
}  // namespace yas::ae
