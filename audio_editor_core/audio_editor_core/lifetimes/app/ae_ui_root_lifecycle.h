//
//  ae_ui_root_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_ui_project_id.h>
#include <ui/yas_ui_standard.h>

#include <map>
#include <string>

namespace yas::ae {
class ui_root_lifetime;

struct ui_root_lifecycle {
    [[nodiscard]] static std::shared_ptr<ui_root_lifecycle> make_shared();

    ui_root_lifecycle();

    void add_lifetime(std::shared_ptr<ui::standard> const &, ui_project_id const &project_id);
    [[nodiscard]] std::shared_ptr<ui_root_lifetime> const &lifetime_for_view_id(uintptr_t const project_view_id) const;
    void remove_lifetime_for_view_id(uintptr_t const project_view_id);

   private:
    std::map<uintptr_t, std::shared_ptr<ui_root_lifetime>> _ui_roots;
};
}  // namespace yas::ae
