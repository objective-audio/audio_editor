//
//  ae_ui_marker_plane.h
//

#pragma once

#include <audio_editor_core/ae_marker_element.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct ui_marker_plane {
    std::shared_ptr<ui::node> const &node() const;

    void set_scale(ui::size const &);
    void set_elements(std::vector<marker_element> &&);

    static std::shared_ptr<ui_marker_plane> make_shared(std::size_t const max_count);

   private:
    ui::size _scale;
    std::vector<marker_element> _elements;

    std::size_t const _max_count;
    std::shared_ptr<ui::node> const _root_node;
    std::vector<std::shared_ptr<ui::node>> _sub_nodes;
    std::shared_ptr<ui::static_mesh_vertex_data> const _vertex_data;
    std::shared_ptr<ui::static_mesh_index_data> const _index_data;

    ui_marker_plane(std::size_t const max_count);

    void _update_sub_nodes();
};
}  // namespace yas::ae
