//
//  ae_ui_root_level_router.h
//

#pragma once

#include <audio_editor_core/ae_ui_project_id.h>
#include <ui/yas_ui_standard.h>

#include <map>
#include <string>

namespace yas::ae {
class ui_root_level;

struct ui_root_level_router {
    [[nodiscard]] static std::shared_ptr<ui_root_level_router> make_shared();

    void add_level(std::shared_ptr<ui::standard> const &, ui_project_id const &project_id);
    [[nodiscard]] std::shared_ptr<ui_root_level> const &level_for_view_id(uintptr_t const project_view_id) const;
    void remove_level_for_view_id(uintptr_t const project_view_id);

   private:
    std::map<uintptr_t, std::shared_ptr<ui_root_level>> _ui_roots;

    ui_root_level_router();
};
}  // namespace yas::ae
