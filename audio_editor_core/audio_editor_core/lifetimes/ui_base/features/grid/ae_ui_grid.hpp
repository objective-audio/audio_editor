//
//  ae_ui_grid.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class grid_presenter;
class color;
class ui_atlas;
class grid_content;

struct ui_grid final {
    [[nodiscard]] static std::shared_ptr<ui_grid> make_shared(window_lifetime_id const &, ui::node *);
    ui_grid(std::shared_ptr<grid_presenter> const &, std::shared_ptr<ui::standard> const &, ae::color *,
            ui_atlas const *, ui::node *);

    void set_scale(ui::size const &);

   private:
    std::shared_ptr<grid_presenter> const _presenter;
    ui::size _scale;
    ui::view_look const *const _view_look;
    ae::color *const _color;
    ui_atlas const *const _atlas;
    ui::node *const _node;

    std::shared_ptr<ui::node> const _lines_node;

    std::size_t _remaked_count = 0;
    std::shared_ptr<ui::dynamic_mesh_vertex_data> _vertex_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _index_data;

    observing::canceller_pool _pool;

    ui_grid(ui_grid const &) = delete;
    ui_grid(ui_grid &&) = delete;
    ui_grid &operator=(ui_grid const &) = delete;
    ui_grid &operator=(ui_grid &&) = delete;

    void _update_lines_scale();
    void _replace_data(std::vector<std::optional<grid_content>> const &);
    void _update_data(std::size_t const count, std::vector<std::pair<std::size_t, grid_content>> const &inserted,
                      std::vector<std::pair<std::size_t, grid_content>> const &replaced,
                      std::vector<std::pair<std::size_t, grid_content>> const &erased);
    void _update_colors(std::vector<std::optional<grid_content>> const &);
    void _remake_data_if_needed(std::size_t const max_count);
    void _set_line_count(std::size_t const line_count);
};
}  // namespace yas::ae
