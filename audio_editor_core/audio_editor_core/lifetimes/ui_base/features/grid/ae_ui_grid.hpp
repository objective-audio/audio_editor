//
//  ae_ui_grid.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <audio_editor_core/ae_ui_types.h>
#include <ui/yas_ui_umbrella.h>

#include <set>

namespace yas::ae {
class grid_presenter;
class color;
class ui_atlas;
class grid_content;
template <typename V, typename I>
class dynamic_mesh_container;

struct ui_grid final {
    [[nodiscard]] static std::shared_ptr<ui_grid> make_shared(project_lifetime_id const &, ui::node *);
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

    std::unique_ptr<dynamic_mesh_container<vertex2d_line, index2d_line>> const _mesh_container;

    observing::canceller_pool _pool;

    ui_grid(ui_grid const &) = delete;
    ui_grid(ui_grid &&) = delete;
    ui_grid &operator=(ui_grid const &) = delete;
    ui_grid &operator=(ui_grid &&) = delete;

    void _update_lines_scale();
    void _replace_data(std::vector<std::optional<grid_content>> const &);
    void _update_data(std::size_t const count, std::set<std::size_t> const &inserted_indices,
                      std::set<std::size_t> const &replaced_indices, std::map<std::size_t, grid_content> const &erased);
    void _update_colors();
    void _update_tex_coords(ui::uint_region const &tex_coords);
};
}  // namespace yas::ae
