//
//  ae_ui_pasting_modules.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class pasting_module_content;
class pasting_modules_presenter;
class display_space;
class color;

struct ui_pasting_modules final {
    [[nodiscard]] static std::shared_ptr<ui_pasting_modules> make_shared(window_lifetime_id const &,
                                                                         std::shared_ptr<ui::node> const &);

    ui_pasting_modules(std::shared_ptr<pasting_modules_presenter> const &, std::shared_ptr<ui::standard> const &,
                       std::shared_ptr<ui::node> const &, ae::color *);

    void set_scale(ui::size const &);

   private:
    std::shared_ptr<pasting_modules_presenter> const _presenter;
    ae::color *const _color;
    std::shared_ptr<ui::node> const _frame_node;

    ui::size _scale{.width = 1.0f, .height = 1.0f};
    std::size_t _remaked_count = 0;
    std::shared_ptr<ui::dynamic_mesh_vertex_data> _vertex_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> _frame_index_data;
    std::shared_ptr<ui::mesh> const _frame_mesh;

    observing::canceller_pool _pool;

    ui_pasting_modules(ui_pasting_modules const &) = delete;
    ui_pasting_modules(ui_pasting_modules &&) = delete;
    ui_pasting_modules &operator=(ui_pasting_modules const &) = delete;
    ui_pasting_modules &operator=(ui_pasting_modules &&) = delete;

    void _set_contents(std::vector<std::optional<pasting_module_content>> const &);
    void _update_contents(std::size_t const count,
                          std::vector<std::pair<std::size_t, pasting_module_content>> const &inserted,
                          std::vector<std::pair<std::size_t, pasting_module_content>> const &replaced,
                          std::vector<std::pair<std::size_t, pasting_module_content>> const &erased);

    void _remake_data_if_needed(std::size_t const max_count);
    void _set_rect_count(std::size_t const rect_count);
};
}  // namespace yas::ae
