//
//  ae_ui_root_level_pool.h
//

#pragma once

#include <ui/yas_ui_standard.h>

#include <map>
#include <string>

namespace yas::ae {
class ui_root_level;

struct ui_root_level_pool {
    [[nodiscard]] static std::shared_ptr<ui_root_level_pool> make_shared();

    void add_level(std::shared_ptr<ui::standard> const &, std::string const &project_id,
                   uintptr_t const project_view_id);
    [[nodiscard]] std::shared_ptr<ui_root_level> const &level_for_view_id(uintptr_t const project_view_id) const;
    void remove_level_for_view_id(uintptr_t const project_view_id);

   private:
    std::map<uintptr_t, std::shared_ptr<ui_root_level>> _ui_roots;

    ui_root_level_pool();
};
}  // namespace yas::ae
