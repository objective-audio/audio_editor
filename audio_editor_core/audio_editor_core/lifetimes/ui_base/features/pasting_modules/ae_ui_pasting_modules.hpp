//
//  ae_ui_pasting_modules.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <audio_editor_core/ae_ui_types.h>
#include <cpp_utils/yas_index_range.h>
#include <ui/yas_ui_umbrella.h>

#include <set>

namespace yas::ae {
class pasting_module_content;
class pasting_modules_presenter;
class color;
template <typename V, typename I>
class dynamic_mesh_container;

struct ui_pasting_modules final {
    [[nodiscard]] static std::shared_ptr<ui_pasting_modules> make_shared(project_lifetime_id const &, ui::node *);

    ui_pasting_modules(std::shared_ptr<pasting_modules_presenter> const &, std::shared_ptr<ui::standard> const &,
                       ui::node *, ae::color *);

    void set_scale(ui::size const &);

   private:
    std::shared_ptr<pasting_modules_presenter> const _presenter;
    ui::node *const _node;
    ae::color *const _color;

    std::shared_ptr<ui::node> const _scale_node;
    std::unique_ptr<dynamic_mesh_container<vertex2d_rect, frame_index2d_rect>> const _mesh_container;

    observing::canceller_pool _pool;

    ui_pasting_modules(ui_pasting_modules const &) = delete;
    ui_pasting_modules(ui_pasting_modules &&) = delete;
    ui_pasting_modules &operator=(ui_pasting_modules const &) = delete;
    ui_pasting_modules &operator=(ui_pasting_modules &&) = delete;

    void _replace();
    void _update_mesh(std::set<std::size_t> const &inserted, std::set<std::size_t> const &replaced_indices,
                      std::map<std::size_t, pasting_module_content> const &erased);
    void _update_y_offset();
};
}  // namespace yas::ae
