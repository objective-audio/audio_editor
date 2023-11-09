//
//  ae_ui_markers.h
//

#pragma once

#include <audio_editor_core/ae_marker_content_pool.h>
#include <audio_editor_core/ae_project_lifetime_id.h>
#include <audio_editor_core/ae_ui_types.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class markers_presenter;
class markers_controller;
class ui_marker_name;
class ui_atlas;
class modifiers_holder;
class color;
template <typename V, typename I>
class dynamic_mesh_container;

struct ui_markers final {
    [[nodiscard]] static std::shared_ptr<ui_markers> make_shared(project_lifetime_id const &,
                                                                 std::shared_ptr<ui::node> const &);

    ui_markers(project_lifetime_id const &, std::shared_ptr<markers_presenter> const &,
               std::shared_ptr<markers_controller> const &, std::shared_ptr<ui::standard> const &, ui::node *,
               modifiers_holder *, ui_atlas const *, ae::color const *);

   private:
    project_lifetime_id const _project_lifetime_id;
    ui::node *const _node;
    std::shared_ptr<ui::node> const _names_root_node;
    std::shared_ptr<markers_presenter> const _presenter;
    std::shared_ptr<markers_controller> const _controller;

    ae::color const *const _color;
    ui_atlas const *const _atlas;
    std::shared_ptr<ui::layout_value_guide> const _top_guide;
    std::unique_ptr<std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>>> _square_vertex_datas;
    std::unique_ptr<dynamic_mesh_container<vertex2d_rect, fill_index2d_rect>> _square_fill_mesh_container;
    std::unique_ptr<std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>>> _line_vertex_datas;
    std::unique_ptr<dynamic_mesh_container<vertex2d_rect, fill_index2d_rect>> _line_mesh_container;
    std::shared_ptr<ui::touch_tracker> const _touch_tracker;
    std::optional<std::size_t> _began_collider_idx;
    std::shared_ptr<ui::multiple_touch> const _multiple_touch;
    modifiers_holder *const _modifiers_holder;
    std::vector<std::unique_ptr<ui_marker_name>> _names;

    observing::canceller_pool _pool;

    ui_markers(ui_markers const &) = delete;
    ui_markers(ui_markers &&) = delete;
    ui_markers &operator=(ui_markers const &) = delete;
    ui_markers &operator=(ui_markers &&) = delete;

    /// contentsを元に全てのUI要素を作り直す
    void _replace_data();
    /// 全てのUI要素の色のみを更新する
    void _update_colors();
    /// UI要素を部分的に更新する
    void _update_data(std::set<std::size_t> const &inserted, std::set<std::size_t> const &replaced,
                      std::map<std::size_t, marker_content> const &erased);

    void _set_rect_count(std::size_t const rect_count);

    void _update_square_region(std::size_t const content_idx, ui::region const &name_region);
    [[nodiscard]] std::function<void(ui::region const &)> _square_region_updated(std::size_t const content_idx);
};
}  // namespace yas::ae
