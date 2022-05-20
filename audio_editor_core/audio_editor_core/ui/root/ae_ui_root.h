//
//  ae_ui_root.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_ui_project_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class root_presenter;
class ui_editing_root;
class color;

struct ui_root final {
    [[nodiscard]] static std::shared_ptr<ui_root> make_shared(std::shared_ptr<ui::standard> const &,
                                                              ui_project_id const &project_id,
                                                              std::shared_ptr<ui_editing_root> const &);

    [[nodiscard]] bool responds_to_action(action const);

   private:
    std::shared_ptr<root_presenter> const _presenter;
    std::shared_ptr<ui::node> const _root_node;
    std::weak_ptr<ae::color> const _color;
    std::weak_ptr<ui::background> const _background;
    std::weak_ptr<ui_editing_root> const _editing_root;
    observing::canceller_pool _pool;

    ui_root(std::shared_ptr<ae::color> const &, std::shared_ptr<ui::standard> const &,
            std::shared_ptr<root_presenter> const &, std::shared_ptr<ui_editing_root> const &);
};
}  // namespace yas::ae
