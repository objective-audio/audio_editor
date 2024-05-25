//
//  ui_framed_square.hpp
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class ui_square_mesh_data;

struct ui_framed_square final {
    ui_framed_square(ui_square_mesh_data const *);

    std::shared_ptr<ui::node> const node;

    void set_region(ui::region const &);
    void set_fill_color(ui::color const &);
    void set_frame_color(ui::color const &);

   private:
    std::shared_ptr<ui::node> const _origin_node;
    std::shared_ptr<ui::node> const _fill_node;
    std::shared_ptr<ui::node> const _line_node;

    ui_framed_square(ui_framed_square const &) = delete;
    ui_framed_square(ui_framed_square &&) = delete;
    ui_framed_square &operator=(ui_framed_square const &) = delete;
    ui_framed_square &operator=(ui_framed_square &&) = delete;
};
}  // namespace yas::ae
