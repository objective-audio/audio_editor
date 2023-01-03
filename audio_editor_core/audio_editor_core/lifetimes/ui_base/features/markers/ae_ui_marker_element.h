//
//  ae_ui_marker_element.hpp
//

#pragma once

#include <audio_editor_core/ae_marker_content.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class marker_pool;
class selected_marker_pool;
class markers_controller;
class modifiers_holder;
class color;
class ui_mesh_data;
class marker_index;
class ui_atlas;

struct ui_marker_element final {
    [[nodiscard]] static std::shared_ptr<ui_marker_element> make_shared(window_lifetime_id const &,
                                                                        ui::node *parent_node);
    ui_marker_element(std::shared_ptr<marker_pool> const &, std::shared_ptr<selected_marker_pool> const &,
                      std::shared_ptr<markers_controller> const &, std::shared_ptr<ui::standard> const &,
                      std::shared_ptr<ae::color> const &, std::shared_ptr<ui_mesh_data> const &vertical_line_data,
                      std::shared_ptr<ui_mesh_data> const &square_data, std::shared_ptr<ui::font_atlas> const &,
                      std::shared_ptr<ui_atlas> const &, ui::node *parent_node, modifiers_holder *);

    void set_content(marker_content const &);
    void update_content(marker_content const &);
    void reset_content();

    void finalize();

   private:
    std::shared_ptr<ui::node> const _node;
    std::optional<marker_content> _content;

    std::weak_ptr<marker_pool> const _marker_pool;
    std::weak_ptr<selected_marker_pool> const _selected_marker_pool;
    std::shared_ptr<markers_controller> const _controller;
    std::shared_ptr<ui::node> const _line_node;
    std::shared_ptr<ui::node> const _square_collider_node;
    std::shared_ptr<ui::rect_plane> const _square_plane;
    std::shared_ptr<ui::strings> const _strings;
    std::shared_ptr<ae::color> const _color;
    std::shared_ptr<ui::collider> const _collider;
    std::shared_ptr<ui::touch_tracker> const _touch_tracker;
    std::shared_ptr<ui::multiple_touch> const _multiple_touch;
    modifiers_holder *const _modifiers_holder;

    observing::canceller_pool _pool;

    ui_marker_element(ui_marker_element const &) = delete;
    ui_marker_element(ui_marker_element &&) = delete;
    ui_marker_element &operator=(ui_marker_element const &) = delete;
    ui_marker_element &operator=(ui_marker_element &&) = delete;

    void _update_name();
    void _update_color();
    std::optional<marker_index> _marker_index() const;
};
}  // namespace yas::ae
