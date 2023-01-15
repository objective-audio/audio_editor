//
//  ae_ui_module_waveforms.h
//

#pragma once

#include <audio_editor_core/ae_module_content_pool.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class module_content;
class module_waveforms_presenter;
class waveform_mesh_importer;
class color;
class ui_atlas;
class waveform_mesh_data;

struct ui_module_waveforms final {
    [[nodiscard]] static std::shared_ptr<ui_module_waveforms> make_shared(window_lifetime_id const &);

    ui_module_waveforms(std::shared_ptr<ui::standard> const &, ae::color *, ui_atlas const *,
                        std::shared_ptr<module_waveforms_presenter> const &);

    std::shared_ptr<ui::node> const node;

    void set_scale(ui::size const &);

    void set_contents(std::vector<std::optional<module_content>> const &, bool const clear_mesh_nodes);
    void update_contents(std::size_t const count, std::vector<std::pair<std::size_t, module_content>> const &erased,
                         std::vector<std::pair<std::size_t, module_content>> const &inserted,
                         std::vector<std::pair<std::size_t, module_content>> const &replaced);

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

    void _resize_elements(std::size_t const);
    std::optional<ui::size> _waveform_scale() const;
    void _update_all_tex_coords(ui::uint_point const &);
    void _update_all_colors(ui::color const &);
};
}  // namespace yas::ae
