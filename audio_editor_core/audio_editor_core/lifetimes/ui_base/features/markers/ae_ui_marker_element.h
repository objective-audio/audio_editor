//
//  ae_ui_marker_element.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class color;
class ui_mesh_data;

struct ui_marker_element final {
    [[nodiscard]] static std::shared_ptr<ui_marker_element> make_shared(window_lifetime_id const &);
    ui_marker_element(std::shared_ptr<ui::standard> const &, std::shared_ptr<ae::color> const &,
                      std::shared_ptr<ui_mesh_data> const &vertical_line_data,
                      std::shared_ptr<ui_mesh_data> const &triangle_data);

    std::shared_ptr<ui::node> const node;

   private:
    std::shared_ptr<ui::node> const _line_node;
    std::shared_ptr<ui::node> const _triangle_node;
    std::shared_ptr<ae::color> const _color;

    observing::canceller_pool _pool;
};
}  // namespace yas::ae
