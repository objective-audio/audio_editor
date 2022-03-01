//
//  ae_ui_edge.h
//

#pragma once

#include <audio_editor_core/ae_ui_project_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class edge_presenter;
class display_space;
class color;
class ui_edge_element;

struct ui_edge final {
    [[nodiscard]] static std::shared_ptr<ui_edge> make_shared(ui_project_id const &project_id);

    std::shared_ptr<ui::node> const &node() const;

   private:
    std::shared_ptr<edge_presenter> const _presenter;

    std::shared_ptr<ui::layout_value_guide> const _top_guide;
    std::shared_ptr<ui::node> const _node;

    std::shared_ptr<ui_edge_element> const _begin_edge;
    std::shared_ptr<ui_edge_element> const _end_edge;

    observing::canceller_pool _pool;

    ui_edge(std::shared_ptr<edge_presenter> const &, std::shared_ptr<ui::standard> const &,
            std::shared_ptr<ui::layout_value_guide> const &top_guide,
            std::shared_ptr<ui_edge_element> const &begin_edge, std::shared_ptr<ui_edge_element> const &end_edge);
};
}  // namespace yas::ae
