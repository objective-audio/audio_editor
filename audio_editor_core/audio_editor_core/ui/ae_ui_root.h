//
//  ae_ui_root.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_ui_root_dependency.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class root_presenter;
class ui_editing_root;
class ui_editing_root_level_pool;

struct ui_root final {
    [[nodiscard]] static std::shared_ptr<ui_root> make_shared(std::shared_ptr<ui::standard> const &,
                                                              std::string const &project_id,
                                                              std::uintptr_t const project_view_id,
                                                              std::shared_ptr<ui_editing_root_level_pool> const &);

    [[nodiscard]] bool responds_to_action(action const);

   private:
    std::uintptr_t const _project_view_id;
    std::shared_ptr<root_presenter> const _presenter;
    std::weak_ptr<ui_editing_root_level_pool> const _ui_editing_root_level_pool;
    observing::canceller_pool _pool;

    ui_root(std::uintptr_t const project_view_id, std::shared_ptr<ui::standard> const &,
            std::shared_ptr<root_presenter> const &, std::shared_ptr<ui_editing_root_level_pool> const &);
};
}  // namespace yas::ae
