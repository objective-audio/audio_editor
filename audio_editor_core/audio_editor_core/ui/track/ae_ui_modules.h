//
//  ae_ui_modules.h
//

#pragma once

#include <audio_editor_core/ae_ui_types.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class module_location;
class modules_presenter;
class display_space;
class ui_module_waveforms;

struct ui_modules final {
    std::shared_ptr<ui::node> const &node() const;

    void set_scale(ui::size const &);
    void set_locations(std::vector<std::optional<module_location>> const &);
    void update_locations(std::size_t const count, std::vector<std::pair<std::size_t, module_location>> const &erased,
                          std::vector<std::pair<std::size_t, module_location>> const &inserted);

    static std::shared_ptr<ui_modules> make_shared(std::string const &project_id, std::shared_ptr<ui::standard> const &,
                                                   std::shared_ptr<display_space> const &);

   private:
    std::shared_ptr<modules_presenter> const _presenter;
    std::shared_ptr<ui_module_waveforms> const _waveforms;
    std::shared_ptr<ui::node> const _node;
    std::shared_ptr<ui::node> const _triangle_node;
    std::shared_ptr<ui::node> const _line_node;

    std::size_t _remaked_count = 0;
    std::shared_ptr<ui::dynamic_mesh_vertex_data> _vertex_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _triangle_index_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _line_index_data;
    std::shared_ptr<ui::mesh> const _triangle_mesh;
    std::shared_ptr<ui::mesh> const _line_mesh;

    observing::canceller_pool _pool;

    ui_modules(std::shared_ptr<modules_presenter> const &, std::shared_ptr<ui::standard> const &,
               std::shared_ptr<ui_module_waveforms> const &);

    void _remake_data_if_needed(std::size_t const);
    void _set_rect_count(std::size_t const);
};
}  // namespace yas::ae
