//
//  ae_ui_time_base.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct ui_time_base final {
    [[nodiscard]] static std::shared_ptr<ui_time_base> make_shared(window_lifetime_id const &,
                                                                   std::shared_ptr<ui::node> const &time_base_node);
    ui_time_base(std::shared_ptr<ui::node> const &time_base_node,
                 std::shared_ptr<ui::font_atlas> const &time_font_atlas, std::shared_ptr<ui::standard> const &);

   private:
    std::shared_ptr<ui::layout_value_guide> const _top_guide;

    observing::canceller_pool _pool;
};
}  // namespace yas::ae
