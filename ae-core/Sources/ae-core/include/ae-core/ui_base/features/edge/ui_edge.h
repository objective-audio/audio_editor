//
//  ui_edge.h
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class edge_presenter;
class display_space;
class color;
class ui_edge_element;
class ui_mesh_data;

struct ui_edge final {
    [[nodiscard]] static std::shared_ptr<ui_edge> make_shared(project_lifetime_id const &,
                                                              std::shared_ptr<ui::node> const &);

    ui_edge(std::shared_ptr<edge_presenter> const &, std::shared_ptr<ui_edge_element> const &begin_edge,
            std::shared_ptr<ui_edge_element> const &end_edge, std::shared_ptr<ui::node> const &);

   private:
    std::shared_ptr<edge_presenter> const _presenter;

    std::shared_ptr<ui_edge_element> const _begin_edge;
    std::shared_ptr<ui_edge_element> const _end_edge;

    observing::canceller_pool _pool;

    ui_edge(ui_edge const &) = delete;
    ui_edge(ui_edge &&) = delete;
    ui_edge &operator=(ui_edge const &) = delete;
    ui_edge &operator=(ui_edge &&) = delete;
};
}  // namespace yas::ae
