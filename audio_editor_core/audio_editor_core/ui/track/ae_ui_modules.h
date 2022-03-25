//
//  ae_ui_modules.h
//

#pragma once

#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_ui_project_id.h>
#include <audio_editor_core/ae_ui_types.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class module_location;
class modules_presenter;
class display_space;
class ui_module_waveforms;
class color;

struct ui_modules final {
    [[nodiscard]] static std::shared_ptr<ui_modules> make_shared(ui_project_id const &project_id,
                                                                 std::shared_ptr<module_location_pool> const &,
                                                                 std::shared_ptr<ui_module_waveforms> const &);

    std::shared_ptr<ui::node> const node;

    void set_scale(ui::size const &);
    void set_locations(std::vector<std::optional<module_location>> const &);
    void update_locations(std::size_t const count, std::vector<std::pair<std::size_t, module_location>> const &erased,
                          std::vector<std::pair<std::size_t, module_location>> const &inserted);

   private:
    std::shared_ptr<modules_presenter> const _presenter;
    std::shared_ptr<ae::color> const _color;
    std::shared_ptr<ui::font_atlas> const _name_font_atlas;
    std::shared_ptr<ui_module_waveforms> const _waveforms;
    std::shared_ptr<ui::node> const _triangle_node;
    std::shared_ptr<ui::node> const _line_node;
    std::shared_ptr<ui::node> const _names_root_node;

    ui::size _scale{.width = 1.0f, .height = 1.0f};
    std::size_t _remaked_count = 0;
    std::shared_ptr<ui::dynamic_mesh_vertex_data> _vertex_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _triangle_index_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _line_index_data;
    std::shared_ptr<ui::mesh> const _triangle_mesh;
    std::shared_ptr<ui::mesh> const _line_mesh;
    std::vector<std::shared_ptr<ui::strings>> _names;

    observing::canceller_pool _pool;

    ui_modules(std::shared_ptr<modules_presenter> const &, std::shared_ptr<ui::standard> const &,
               std::shared_ptr<ae::color> const &, std::shared_ptr<ui::font_atlas> const &,
               std::shared_ptr<ui_module_waveforms> const &);

    void _remake_data_if_needed(std::size_t const);
    void _set_rect_count(std::size_t const);
    void _update_all_name_positions();
    void _update_name_position(std::size_t const idx, ae::module_location const &);
};
}  // namespace yas::ae
