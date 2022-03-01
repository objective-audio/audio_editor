//
//  ae_ui_editing_root_level.h
//

#pragma once

#include <memory>

namespace yas::ae {
class ui_editing_root;

struct ui_editing_root_level final {
    [[nodiscard]] static std::shared_ptr<ui_editing_root_level> make_shared(std::string const &project_id,
                                                                            std::uintptr_t const project_view_id);

    std::shared_ptr<ui_editing_root> const ui_editing_root;

   private:
    ui_editing_root_level(std::string const &project_id, std::uintptr_t const project_view_id);
};
}  // namespace yas::ae
