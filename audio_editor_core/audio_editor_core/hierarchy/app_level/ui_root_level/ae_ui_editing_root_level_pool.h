//
//  ae_ui_editing_root_level_pool.h
//

#pragma once

#include <memory>
#include <string>

namespace yas::ae {
class ui_editing_root_level;

struct ui_editing_root_level_pool final {
    [[nodiscard]] static std::shared_ptr<ui_editing_root_level_pool> make_shared(std::string const &project_id,
                                                                                 std::uintptr_t const project_view_id);

    void add_level();

    std::shared_ptr<ui_editing_root_level> const &level() const;

   private:
    std::string const _project_id;
    std::uintptr_t const _project_view_id;

    std::shared_ptr<ui_editing_root_level> _level;

    ui_editing_root_level_pool(std::string const &project_id, std::uintptr_t const project_view_id);
};
}  // namespace yas::ae
