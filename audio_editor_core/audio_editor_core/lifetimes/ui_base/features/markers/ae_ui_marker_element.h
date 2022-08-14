//
//  ae_ui_marker_element.hpp
//

#pragma once

#include <audio_editor_core/ae_marker_location.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <cpp_utils/yas_identifier.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class marker_pool;
class markers_controller;
class color;
class ui_mesh_data;

struct ui_marker_element final {
    [[nodiscard]] static std::shared_ptr<ui_marker_element> make_shared(window_lifetime_id const &,
                                                                        ui::node *parent_node);
    ui_marker_element(std::shared_ptr<marker_pool> const &, std::shared_ptr<markers_controller> const &,
                      std::shared_ptr<ui::standard> const &, std::shared_ptr<ae::color> const &,
                      std::shared_ptr<ui_mesh_data> const &vertical_line_data,
                      std::shared_ptr<ui_mesh_data> const &square_data, std::shared_ptr<ui::font_atlas> const &,
                      ui::node *parent_node);

    std::shared_ptr<ui::node> const node;

    void set_location(marker_location const &);
    void reset_location();

    void finalize();

   private:
    std::optional<identifier> _identifier;

    std::weak_ptr<marker_pool> const _marker_pool;
    std::shared_ptr<markers_controller> const _controller;
    std::shared_ptr<ui::node> const _line_node;
    std::shared_ptr<ui::node> const _square_collider_node;
    std::shared_ptr<ui::node> const _square_mesh_node;
    std::shared_ptr<ui::strings> const _strings;
    std::shared_ptr<ae::color> const _color;
    std::shared_ptr<ui::collider> const _collider;
    std::shared_ptr<ui::touch_tracker> const _touch_tracker;
    std::shared_ptr<ui::multiple_touch> const _multiple_touch;

    observing::canceller_pool _pool;

    void _update_name();
};
}  // namespace yas::ae
