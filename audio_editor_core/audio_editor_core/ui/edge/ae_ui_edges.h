//
//  ae_ui_edges.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class edge_presenter;
class display_space;
class color;
class ui_edge;

struct ui_edges final {
    [[nodiscard]] static std::shared_ptr<ui_edges> make_shared(std::string const &project_id,
                                                               uintptr_t const project_view_id);

    std::shared_ptr<ui::node> const &node() const;

   private:
    std::shared_ptr<edge_presenter> const _presenter;

    std::shared_ptr<ui::layout_value_guide> const _top_guide;
    std::shared_ptr<ui::node> const _node;

    std::shared_ptr<ui_edge> const _begin_edge;
    std::shared_ptr<ui_edge> const _end_edge;

    observing::canceller_pool _pool;

    ui_edges(std::shared_ptr<edge_presenter> const &, std::shared_ptr<ui::standard> const &,
             std::shared_ptr<ui::layout_value_guide> const &top_guide, std::shared_ptr<ui_edge> const &begin_edge,
             std::shared_ptr<ui_edge> const &end_edge);
};
}  // namespace yas::ae
