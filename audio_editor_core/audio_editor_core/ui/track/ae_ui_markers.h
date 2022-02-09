//
//  ae_ui_markers.h
//

#pragma once

#include <audio_editor_core/ae_marker_location.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class markers_presenter;
class display_space;
class color;

struct ui_markers final {
    std::shared_ptr<ui::node> const &node() const;

    void set_locations(std::vector<std::optional<marker_location>> const &);
    void update_locations(std::size_t const count, std::vector<std::pair<std::size_t, marker_location>> const &erased,
                          std::vector<std::pair<std::size_t, marker_location>> const &inserted);

    static std::shared_ptr<ui_markers> make_shared(std::string const &project_id, std::shared_ptr<ui::standard> const &,
                                                   std::shared_ptr<display_space> const &);

   private:
    std::shared_ptr<markers_presenter> const _presenter;
    std::shared_ptr<ae::color> const _color;

    ui::size _scale;

    std::shared_ptr<ui::node> const _root_node;
    std::vector<std::shared_ptr<ui::node>> _sub_nodes;
    std::shared_ptr<ui::static_mesh_vertex_data> const _vertex_data;
    std::shared_ptr<ui::static_mesh_index_data> const _index_data;

    observing::canceller_pool _pool;

    ui_markers(std::shared_ptr<markers_presenter> const &, std::shared_ptr<ui::standard> const &,
               std::shared_ptr<ae::color> const &);

    void _set_count(std::size_t const);
};
}  // namespace yas::ae
