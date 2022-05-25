//
//  ae_ui_modal_bg.h
//

#pragma once

#include <audio_editor_core/ae_ui_project_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class color;
class project_sub_level_router;
class project_action_controller;

struct ui_modal_bg {
    [[nodiscard]] static std::shared_ptr<ui_modal_bg> make_shared(ui_project_id const &,
                                                                  std::shared_ptr<ui::standard> const &);

    ui_modal_bg(std::shared_ptr<ui::standard> const &, std::shared_ptr<ae::color> const &,
                std::shared_ptr<project_sub_level_router> const &, std::shared_ptr<project_action_controller> const &);

    std::shared_ptr<ui::node> const node;

   private:
    std::shared_ptr<ae::color> const _color;
    std::shared_ptr<ui::button> const _button;
    std::weak_ptr<project_action_controller> const _action_controller;

    observing::canceller_pool _pool;
};
}  // namespace yas::ae
