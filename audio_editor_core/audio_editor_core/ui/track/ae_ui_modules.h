//
//  ae_ui_modules.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class module_element;

struct ui_modules {
    using vertex2d_rect = ui::vertex2d_rect;
    using triangle_index2d_rect = ui::index2d_rect;
    struct line_index2d_rect {
        ui::index2d_t v[8];

        void set_all(ui::index2d_t const first);
    };

    std::shared_ptr<ui::node> const &node() const;

    void set_scale(ui::size const &);
    void set_elements(std::vector<module_element> const &);

    static std::shared_ptr<ui_modules> make_shared(std::size_t const max_count);

   private:
    std::size_t const _max_count;
    std::shared_ptr<ui::node> const _node;
    std::shared_ptr<ui::node> const _triangle_node;
    std::shared_ptr<ui::node> const _line_node;
    std::shared_ptr<ui::dynamic_mesh_vertex_data> _vertex_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _triangle_index_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _line_index_data;
    std::shared_ptr<ui::mesh> const _triangle_mesh;
    std::shared_ptr<ui::mesh> const _line_mesh;

    ui_modules(std::size_t const max_count);

    void _set_rect_count(std::size_t const);
    void _write_vertices(std::function<void(vertex2d_rect *)> const &);
    void _write_triangle_indices(std::function<void(triangle_index2d_rect *)> const &);
    void _write_line_indices(std::function<void(line_index2d_rect *)> const &);
};
}  // namespace yas::ae
