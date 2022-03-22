//
//  ae_ui_edge_element.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class color;
class ui_mesh_data;

struct ui_edge_element final {
    struct args final {
        std::shared_ptr<ui_mesh_data> const &vertical_line_data;
    };

    [[nodiscard]] static std::shared_ptr<ui_edge_element> make_shared(std::string const &text, args const &args,
                                                                      uintptr_t const project_view_id);

    std::shared_ptr<ui::node> const node;

   private:
    std::shared_ptr<ui::node> const _line_node;
    std::shared_ptr<ui::strings> const _text;
    std::shared_ptr<ae::color> const _color;

    observing::canceller_pool _pool;

    ui_edge_element(std::string const &text, std::shared_ptr<ui::standard> const &,
                    std::shared_ptr<ui::font_atlas> const &, std::shared_ptr<ae::color> const &, args const &args);
};
}  // namespace yas::ae
