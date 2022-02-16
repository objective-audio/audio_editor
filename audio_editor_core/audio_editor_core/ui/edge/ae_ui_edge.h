//
//  ae_ui_edge.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class edge_presenter;
class display_space;
class color;

struct ui_edge final {
    [[nodiscard]] static std::shared_ptr<ui_edge> make_shared(std::string const &project_id,
                                                              uintptr_t const project_view_id);

    std::shared_ptr<ui::node> const &node() const;

   private:
    std::shared_ptr<edge_presenter> const _presenter;
    std::shared_ptr<ae::color> const _color;

    std::shared_ptr<ui::node> const _node;

    std::shared_ptr<ui::static_mesh_vertex_data> _vertex_data;
    std::shared_ptr<ui::static_mesh_index_data> _index_data;
    std::shared_ptr<ui::mesh> const _begin_mesh;
    std::shared_ptr<ui::node> const _begin_node;
    std::shared_ptr<ui::mesh> const _end_mesh;
    std::shared_ptr<ui::node> const _end_node;

    observing::canceller_pool _pool;

    ui_edge(std::shared_ptr<edge_presenter> const &, std::shared_ptr<ui::standard> const &,
            std::shared_ptr<ae::color> const &);

    void _setup_nodes();
    void _setup_observing(std::shared_ptr<ui::standard> const &standard);
};
}  // namespace yas::ae
