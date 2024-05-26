//
//  ui_module_waveforms.h
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ae-core/project_editing/features/module_content_pool.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class module_content;
class module_waveforms_presenter;
class waveform_mesh_importer;
class color;
class ui_atlas;
class waveform_mesh_data;

struct ui_module_waveforms final {
    [[nodiscard]] static std::shared_ptr<ui_module_waveforms> make_shared(project_lifetime_id const &);

    ui_module_waveforms(std::shared_ptr<ui::standard> const &, ae::color *, ui_atlas const *,
                        std::shared_ptr<module_waveforms_presenter> const &);

    std::shared_ptr<ui::node> const node;

    void set_scale(ui::size const &);

   private:
    std::shared_ptr<module_waveforms_presenter> const _presenter;
    ae::color *const _color;
    ui_atlas const *const _atlas;
    std::optional<float> _scale = std::nullopt;

    struct element;
    std::vector<std::shared_ptr<element>> _elements;

    observing::canceller_pool _pool;

    ui_module_waveforms(ui_module_waveforms const &) = delete;
    ui_module_waveforms(ui_module_waveforms &&) = delete;
    ui_module_waveforms &operator=(ui_module_waveforms const &) = delete;
    ui_module_waveforms &operator=(ui_module_waveforms &&) = delete;

    void _replace_elements(bool const clear_mesh_nodes);
    void _update_elements(std::set<std::size_t> const &inserted, std::set<std::size_t> const &replaced,
                          std::map<std::size_t, module_content> const &erased);
    void _resize_elements(std::size_t const);
    std::optional<ui::size> _waveform_scale() const;
    void _update_all_tex_coords(ui::uint_point const &);
    void _update_all_colors();
};
}  // namespace yas::ae
