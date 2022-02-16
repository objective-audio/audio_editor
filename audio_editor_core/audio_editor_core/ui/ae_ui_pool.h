//
//  ae_ui_pool.h
//

#pragma once

#include <ui/yas_ui_standard.h>

#include <map>
#include <string>

namespace yas::ae {
class ui_root;

struct ui_pool {
    [[nodiscard]] static std::shared_ptr<ui_pool> make_shared();

    [[nodiscard]] std::shared_ptr<ui_root> const &add_and_return_ui_root(std::shared_ptr<ui::standard> const &,
                                                                         std::string const &project_id,
                                                                         uintptr_t const project_view_id);
    [[nodiscard]] std::shared_ptr<ui_root> const &ui_root_for_view_id(uintptr_t const project_view_id) const;
    void remove_ui_root_for_view_id(uintptr_t const project_view_id);

   private:
    std::map<uintptr_t, std::shared_ptr<ui_root>> _ui_roots;

    ui_pool();
};
}  // namespace yas::ae
