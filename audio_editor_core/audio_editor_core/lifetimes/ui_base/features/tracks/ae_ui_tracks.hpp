//
//  ae_ui_tracks.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <audio_editor_core/ae_track_content_pool.h>
#include <audio_editor_core/ae_ui_types.h>
#include <ui/yas_ui_umbrella.h>

#include <audio_editor_core/ae_ui_dynamic_mesh_container.hpp>

namespace yas::ae {
class tracks_presenter;
class tracks_controller;
class color;
class ui_atlas;
class modifiers_holder;

struct ui_tracks {
    [[nodiscard]] static std::shared_ptr<ui_tracks> make_shared(project_lifetime_id const &, ui::node *);

    ui_tracks(project_lifetime_id const &, ui::node *, std::shared_ptr<tracks_presenter> const &,
              std::shared_ptr<tracks_controller> const &, std::shared_ptr<ui::standard> const &, ae::color *,
              modifiers_holder *, ui_atlas const *);

    void set_scale(ui::size const &);

   private:
    project_lifetime_id const _project_lifetime_id;
    ui::node *const _node;
    std::shared_ptr<ui::node> const _names_root_node;
    std::shared_ptr<tracks_presenter> const _presenter;
    std::shared_ptr<tracks_controller> const _controller;
    ae::color *const _color;
    ui_atlas const *const _atlas;
    std::shared_ptr<ui::layout_value_guide> const _left_guide;
    std::unique_ptr<std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>>> _vertex_datas;
    std::unique_ptr<dynamic_mesh_container<vertex2d_rect, fill_index2d_rect>> _fill_mesh_container;
    std::shared_ptr<ui::touch_tracker> const _touch_tracker;
    std::optional<std::size_t> _began_collider_idx;
    modifiers_holder *const _modifiers_holder;

    ui::size _scale = ui::size::one();

    observing::canceller_pool _pool;

    ui_tracks(ui_tracks const &) = delete;
    ui_tracks(ui_tracks &&) = delete;
    ui_tracks &operator=(ui_tracks const &) = delete;
    ui_tracks &operator=(ui_tracks &&) = delete;

    /// contentsを元に全てのUI要素を作り直す
    void _replace_data();
    /// 全てのUI要素の色のみを更新する
    void _update_colors();
    /// UI要素を部分的に更新する
    void _update_data(std::set<std::size_t> const &inserted, std::set<std::size_t> const &replaced,
                      std::map<std::size_t, track_content> const &erased);
    void _set_rect_count(std::size_t const rect_count);
};
}  // namespace yas::ae
