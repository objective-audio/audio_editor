//
//  ae_ui_modules.h
//

#pragma once

#include <audio_editor_core/ae_module_content_pool.h>
#include <audio_editor_core/ae_project_lifetime_id.h>
#include <audio_editor_core/ae_ui_types.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class modules_presenter;
class modules_controller;
class display_space;
class ui_module_waveforms;
class modifiers_holder;
class color;
class ui_atlas;
template <typename V, typename I>
class dynamic_mesh_container;

struct ui_modules final {
    [[nodiscard]] static std::shared_ptr<ui_modules> make_shared(project_lifetime_id const &,
                                                                 std::shared_ptr<ui::node> const &,
                                                                 ui_module_waveforms *);

    ui_modules(std::shared_ptr<modules_presenter> const &, std::shared_ptr<modules_controller> const &,
               std::shared_ptr<ui::standard> const &, std::shared_ptr<ui::node> const &, ae::color *,
               std::shared_ptr<ui::font_atlas> const &, ui_module_waveforms *, modifiers_holder *, ui_atlas const *);

    void set_scale(ui::size const &);

   private:
    std::shared_ptr<modules_presenter> const _presenter;
    std::shared_ptr<modules_controller> const _controller;
    ae::color *const _color;
    std::shared_ptr<ui::font_atlas> const _name_font_atlas;
    ui_atlas const *const _atlas;
    ui_module_waveforms *const _waveforms;
    std::unique_ptr<std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>>> _vertex_datas;
    std::unique_ptr<dynamic_mesh_container<vertex2d_rect, fill_index2d_rect>> _fill_mesh_container;
    std::unique_ptr<dynamic_mesh_container<vertex2d_rect, frame_index2d_rect>> _frame_mesh_container;
    std::shared_ptr<ui::node> const _names_root_node;
    std::shared_ptr<ui::touch_tracker> const _touch_tracker;
    std::optional<std::size_t> _began_collider_idx;
    std::shared_ptr<ui::multiple_touch> const _multiple_touch;
    modifiers_holder *const _modifiers_holder;

    ui::size _scale = ui::size::one();
    std::vector<std::shared_ptr<ui::strings>> _names;

    observing::canceller_pool _pool;

    ui_modules(ui_modules const &) = delete;
    ui_modules(ui_modules &&) = delete;
    ui_modules &operator=(ui_modules const &) = delete;
    ui_modules &operator=(ui_modules &&) = delete;

    /// contentsを元に全てのUI要素を作り直す
    void _replace_data();
    /// 全てのUI要素の色のみを更新する
    void _update_colors();
    /// UI要素を部分的に更新する
    void _update_data(std::set<std::size_t> const &inserted, std::set<std::size_t> const &replaced_indices,
                      std::map<std::size_t, module_content> const &erased);

    void _set_rect_count(std::size_t const rect_count);
    void _update_all_name_positions();
    void _update_name_position(std::size_t const idx, ae::module_content const &);
};
}  // namespace yas::ae
