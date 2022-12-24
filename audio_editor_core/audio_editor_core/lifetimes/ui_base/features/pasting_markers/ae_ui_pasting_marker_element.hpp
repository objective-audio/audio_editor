//
//  ae_ui_pasting_marker_element.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

#include <audio_editor_core/ae_pasting_marker_content.hpp>

namespace yas::ae {
class color;
class ui_mesh_data;

struct ui_pasting_marker_element final {
    [[nodiscard]] static std::shared_ptr<ui_pasting_marker_element> make_shared(window_lifetime_id const &,
                                                                                ui::node *parent_node);

    ui_pasting_marker_element(std::shared_ptr<ui::standard> const &, ui::node *parent_node,
                              std::shared_ptr<ui_mesh_data> const &vertical_line_data);

    void set_content(pasting_marker_content const &);
    void update_content(pasting_marker_content const &);
    void reset_content();

    void finalize();

   private:
    std::shared_ptr<ui::node> const _node;
    std::optional<pasting_marker_content> _content;

    std::shared_ptr<ui::node> const _line_node;
    std::shared_ptr<ae::color> const _color;

    observing::canceller_pool _pool;

    void _update_color();
};
}  // namespace yas::ae
