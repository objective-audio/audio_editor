//
//  ae_ui_edge.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class color;

struct ui_edge final {
    struct args final {
        std::shared_ptr<ui::static_mesh_vertex_data> const &line_vertex_data;
        std::shared_ptr<ui::static_mesh_index_data> const &line_index_data;
        std::shared_ptr<ui::layout_value_guide> const &top_guide;
    };

    [[nodiscard]] static std::shared_ptr<ui_edge> make_shared(std::string const &text, args const &args,
                                                              uintptr_t const project_view_id);

    [[nodiscard]] std::shared_ptr<ui::node> const &node() const;

   private:
    std::shared_ptr<ui::mesh> const _mesh;
    std::shared_ptr<ui::node> const _node;
    std::shared_ptr<ui::node> const _line_node;
    std::shared_ptr<ui::strings> const _text;
    std::shared_ptr<ae::color> const _color;

    observing::canceller_pool _pool;

    ui_edge(std::string const &text, std::shared_ptr<ui::standard> const &, std::shared_ptr<ui::font_atlas> const &,
            std::shared_ptr<ae::color> const &, args const &args);
};
}  // namespace yas::ae
