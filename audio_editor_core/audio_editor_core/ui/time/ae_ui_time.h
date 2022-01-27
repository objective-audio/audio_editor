//
//  ae_ui_time.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class time_presenter;

struct ui_time final {
    [[nodiscard]] static std::shared_ptr<ui_time> make_shared(std::shared_ptr<ui::standard> const &,
                                                              std::shared_ptr<ui::texture> const &,
                                                              std::string const &project_id);

    std::shared_ptr<ui::layout_value_target> top_layout_target() const;
    std::shared_ptr<ui::node> const &node() const;

   private:
    std::shared_ptr<time_presenter> const _presenter;
    std::shared_ptr<ui::font_atlas> const _font_atlas;
    std::shared_ptr<ui::layout_value_guide> const _top_guide;
    std::shared_ptr<ui::node> const _node;
    std::shared_ptr<ui::rect_plane> const _bg;
    std::shared_ptr<ui::strings> const _time_strings;
    std::shared_ptr<ui::rect_plane> const _editing_frame;

    observing::canceller_pool _pool;

    ui_time(std::shared_ptr<ui::standard> const &, std::shared_ptr<ui::texture> const &,
            std::shared_ptr<time_presenter> const &);

    void _update_editing_frame();
};
}  // namespace yas::ae
