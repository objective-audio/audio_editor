//
//  ae_ui_marker_element.hpp
//

#pragma once

#include <audio_editor_core/ae_marker_content.h>
#include <audio_editor_core/ae_project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

#include <audio_editor_core/ae_selected_marker_pool.hpp>

namespace yas::ae {
class marker_pool;
class marker_element_controller;
class modifiers_holder;
class color;
class marker_index;
class ui_square_mesh_data;

struct ui_marker_element final {
    [[nodiscard]] static std::shared_ptr<ui_marker_element> make_shared(project_lifetime_id const &,
                                                                        ui::node *parent_node);
    ui_marker_element(std::shared_ptr<marker_pool> const &, std::shared_ptr<selected_marker_pool> const &,
                      std::shared_ptr<marker_element_controller> const &, std::shared_ptr<ui::standard> const &,
                      std::shared_ptr<ae::color> const &, std::shared_ptr<ui_square_mesh_data> const &square_data,
                      std::shared_ptr<ui::font_atlas> const &, ui::node *parent_node, modifiers_holder *);

    void set_content(marker_content const &);
    void update_content(marker_content const &);
    void reset_content();

    [[nodiscard]] std::optional<marker_index> marker_index() const;
    [[nodiscard]] bool hit_test(ui::region const &);

    void finalize();

   private:
    std::shared_ptr<ui::node> const _node;
    std::optional<marker_content> _content;

    std::weak_ptr<marker_pool> const _marker_pool;
    std::weak_ptr<selected_marker_pool> const _selected_marker_pool;
    std::shared_ptr<marker_element_controller> const _controller;
    std::shared_ptr<ui::node> const _line_node;
    std::shared_ptr<ui::node> const _square_collider_node;
    std::shared_ptr<ui::node> const _square_node;
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
};
}  // namespace yas::ae
