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
class display_space;

struct ui_root final {
    [[nodiscard]] static std::shared_ptr<ui_root> make_shared(std::shared_ptr<ui::standard> const &,
                                                              std::string const &project_id,
                                                              std::uintptr_t const project_view_id);

    [[nodiscard]] bool responds_to_action(action const);

   private:
    std::uintptr_t const _project_view_id;
    std::shared_ptr<root_presenter> const _presenter;
    std::shared_ptr<ui_editing_root_maker_for_ui_root> const _editing_root_maker;
    std::shared_ptr<ui_editing_root> _editing_root = nullptr;
    observing::canceller_pool _pool;

    ui_root(std::uintptr_t const project_view_id, std::shared_ptr<ui::standard> const &,
            std::shared_ptr<root_presenter> const &, std::shared_ptr<ui_editing_root_maker_for_ui_root> const &);
};
}  // namespace yas::ae
