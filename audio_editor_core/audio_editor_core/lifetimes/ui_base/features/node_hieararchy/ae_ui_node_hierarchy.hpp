//
//  ae_ui_node_hierarchy.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct ui_node_hierarchy final {
    [[nodiscard]] static std::shared_ptr<ui_node_hierarchy> make_shared(window_lifetime_id const &);
    ui_node_hierarchy(std::shared_ptr<ui::node> const &root_node);

    std::shared_ptr<ui::node> const base_node;

    std::shared_ptr<ui::node> const scroller_node;
    std::shared_ptr<ui::node> const pasting_modules_node;
    std::shared_ptr<ui::node> const pasting_markers_node;
    std::shared_ptr<ui::node> const playing_line_node;
    std::shared_ptr<ui::node> const modal_bg_node;
    std::shared_ptr<ui::node> const time_base_node;

    // scroller
    std::shared_ptr<ui::node> const track_node;
    std::shared_ptr<ui::node> const modules_node;
    std::shared_ptr<ui::node> const edge_node;
    std::shared_ptr<ui::node> const markers_node;

    // time
    std::shared_ptr<ui::node> const time_bg_node;
    std::shared_ptr<ui::node> const time_buttons_node;
    std::shared_ptr<ui::node> const time_nudge_node;
    std::shared_ptr<ui::node> const time_strings_node;

   private:
    ui_node_hierarchy(ui_node_hierarchy const &) = delete;
    ui_node_hierarchy(ui_node_hierarchy &&) = delete;
    ui_node_hierarchy &operator=(ui_node_hierarchy const &) = delete;
    ui_node_hierarchy &operator=(ui_node_hierarchy &&) = delete;
};
}  // namespace yas::ae
