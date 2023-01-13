//
//  ae_ui_modules.h
//

#pragma once

#include <audio_editor_core/ae_module_content_pool.h>
#include <audio_editor_core/ae_ui_types.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class module_content;
class modules_presenter;
class modules_controller;
class display_space;
class ui_module_waveforms;
class modifiers_holder;
class color;
class ui_atlas;

struct ui_modules final {
    [[nodiscard]] static std::shared_ptr<ui_modules> make_shared(window_lifetime_id const &,
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
    std::shared_ptr<ui::node> const _triangle_node;
    std::shared_ptr<ui::node> const _line_node;
    std::shared_ptr<ui::node> const _names_root_node;
    std::shared_ptr<ui::touch_tracker> const _touch_tracker;
    std::shared_ptr<ui::multiple_touch> const _multiple_touch;
    modifiers_holder *const _modifiers_holder;

    ui::size _scale{.width = 1.0f, .height = 1.0f};
    std::size_t _remaked_count = 0;
    std::shared_ptr<ui::dynamic_mesh_vertex_data> _vertex_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _triangle_index_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _line_index_data;
    std::shared_ptr<ui::mesh> const _triangle_mesh;
    std::shared_ptr<ui::mesh> const _line_mesh;
    std::vector<std::shared_ptr<ui::strings>> _names;

    observing::canceller_pool _pool;

    ui_modules(ui_modules const &) = delete;
    ui_modules(ui_modules &&) = delete;
    ui_modules &operator=(ui_modules const &) = delete;
    ui_modules &operator=(ui_modules &&) = delete;

    void _set_contents(std::vector<std::optional<module_content>> const &);
    void _update_contents(std::size_t const count, std::vector<std::pair<std::size_t, module_content>> const &inserted,
                          std::vector<std::pair<std::size_t, module_content>> const &replaced,
                          std::vector<std::pair<std::size_t, module_content>> const &erased);

    void _remake_data_if_needed(std::size_t const max_count);
    void _set_rect_count(std::size_t const rect_count);
    void _update_all_name_positions();
    void _update_name_position(std::size_t const idx, ae::module_content const &);
};
}  // namespace yas::ae
