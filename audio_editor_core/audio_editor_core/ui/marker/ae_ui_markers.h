//
//  ae_ui_markers.h
//

#pragma once

#include <audio_editor_core/ae_marker_location.h>
#include <audio_editor_core/ae_ui_project_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class markers_presenter;
class display_space;
class color;
class ui_marker_element;
class ui_mesh_data;

struct ui_markers final {
    [[nodiscard]] static std::shared_ptr<ui_markers> make_shared(ui_project_id const &project_id);

    std::shared_ptr<ui::node> const node;

    void set_locations(std::vector<std::optional<marker_location>> const &);
    void update_locations(std::size_t const count, std::vector<std::pair<std::size_t, marker_location>> const &erased,
                          std::vector<std::pair<std::size_t, marker_location>> const &inserted);

   private:
    uintptr_t const _project_view_id;
    std::shared_ptr<markers_presenter> const _presenter;
    std::shared_ptr<ae::color> const _color;

    std::shared_ptr<ui::layout_value_guide> const _top_guide;
    std::vector<std::shared_ptr<ui_marker_element>> _elements;
    std::shared_ptr<ui_mesh_data> const _vertical_line_data;
    std::shared_ptr<ui_mesh_data> const _triangle_data;

    observing::canceller_pool _pool;

    ui_markers(uintptr_t const project_view_id, std::shared_ptr<markers_presenter> const &,
               std::shared_ptr<ui::standard> const &, std::shared_ptr<ae::color> const &,
               std::shared_ptr<ui_mesh_data> const &);

    void _set_count(std::size_t const);
};
}  // namespace yas::ae