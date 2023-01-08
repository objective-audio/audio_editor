//
//  ae_ui_time_bg.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class project_action_sender;
class color;
class ui_atlas;

struct ui_time_bg final {
    [[nodiscard]] static std::shared_ptr<ui_time_bg> make_shared(
        window_lifetime_id const &, std::shared_ptr<ui::node> const &,
        std::shared_ptr<ui::layout_region_source> const &time_strings_layout);
    ui_time_bg(std::shared_ptr<project_action_sender> const &, color *color, std::shared_ptr<ui::standard> const &,
               std::shared_ptr<ui::node> const &, std::shared_ptr<ui::layout_region_source> const &time_strings_layout,
               ui_atlas const *);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;
    ae::color *const _color;
    std::shared_ptr<ui::button> const _button;

    observing::canceller_pool _pool;

    ui_time_bg(ui_time_bg const &) = delete;
    ui_time_bg(ui_time_bg &&) = delete;
    ui_time_bg &operator=(ui_time_bg const &) = delete;
    ui_time_bg &operator=(ui_time_bg &&) = delete;
};
}  // namespace yas::ae
