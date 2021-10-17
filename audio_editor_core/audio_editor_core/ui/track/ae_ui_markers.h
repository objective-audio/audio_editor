//
//  ae_ui_markers.h
//

#pragma once

#include <audio_editor_core/ae_marker_location.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class markers_presenter;

struct ui_markers {
    std::shared_ptr<ui::node> const &node() const;

    void set_scale(ui::size const &);
    void set_locations(std::vector<marker_location> const &);

    static std::shared_ptr<ui_markers> make_shared(std::string const &project_id);

   private:
    std::shared_ptr<markers_presenter> const _presenter;

    ui::size _scale;
    std::vector<marker_location> _locations;

    std::shared_ptr<ui::node> const _root_node;
    std::vector<std::shared_ptr<ui::node>> _sub_nodes;
    std::shared_ptr<ui::static_mesh_vertex_data> const _vertex_data;
    std::shared_ptr<ui::static_mesh_index_data> const _index_data;

    observing::canceller_pool _pool;

    ui_markers(std::shared_ptr<markers_presenter> const &);

    void _update_sub_nodes();
};
}  // namespace yas::ae
