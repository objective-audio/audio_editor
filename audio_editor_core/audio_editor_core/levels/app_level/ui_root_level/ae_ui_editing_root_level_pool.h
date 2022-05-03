//
//  ae_ui_editing_root_level_pool.h
//

#pragma once

#include <audio_editor_core/ae_ui_project_id.h>

#include <memory>
#include <string>

namespace yas::ae {
class ui_editing_root_level;

struct ui_editing_root_level_pool final {
    [[nodiscard]] static std::shared_ptr<ui_editing_root_level_pool> make_shared(ui_project_id const &project_id);

    void add_level();
    void remove_level();

    std::shared_ptr<ui_editing_root_level> const &level() const;

   private:
    ui_project_id const _project_id;

    std::shared_ptr<ui_editing_root_level> _level;

    ui_editing_root_level_pool(ui_project_id const &project_id);
};
}  // namespace yas::ae
