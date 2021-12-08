//
//  ae_ui_modules.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class module_location;
class modules_presenter;
class display_space;

struct ui_modules {
    using vertex2d_rect = ui::vertex2d_rect;
    using triangle_index2d_rect = ui::index2d_rect;
    struct line_index2d_rect {
        static std::size_t constexpr vector_count = 8;

        ui::index2d_t v[vector_count];

        void set_all(ui::index2d_t const first);
    };

    std::shared_ptr<ui::node> const &node() const;

    void set_scale(ui::size const &);
    void set_locations(std::vector<std::optional<module_location>> const &);
    void update_locations(std::size_t const count, std::vector<std::pair<std::size_t, module_location>> const &erased,
                          std::vector<std::pair<std::size_t, module_location>> const &inserted);

    static std::shared_ptr<ui_modules> make_shared(std::string const &project_id,
                                                   std::shared_ptr<display_space> const &);

   private:
    std::shared_ptr<modules_presenter> const _presenter;
    std::shared_ptr<ui::node> const _node;
    std::shared_ptr<ui::node> const _triangle_node;
    std::shared_ptr<ui::node> const _line_node;

    std::size_t _remaked_count = 0;
    std::shared_ptr<ui::dynamic_mesh_vertex_data> _vertex_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _triangle_index_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _line_index_data;
    std::shared_ptr<ui::mesh> const _triangle_mesh;
    std::shared_ptr<ui::mesh> const _line_mesh;

    observing::canceller_pool _pool;

    ui_modules(std::shared_ptr<modules_presenter> const &);

    void _remake_data_if_needed(std::size_t const);
    void _set_rect_count(std::size_t const);
};
}  // namespace yas::ae
