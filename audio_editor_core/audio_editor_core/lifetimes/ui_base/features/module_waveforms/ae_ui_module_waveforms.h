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

struct ui_module_waveforms final {
    [[nodiscard]] static std::shared_ptr<ui_module_waveforms> make_shared(window_lifetime_id const &);

    ui_module_waveforms(std::shared_ptr<ui::standard> const &, ae::color *,
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
    std::optional<float> _scale = std::nullopt;

    observing::canceller_pool _pool;

    void _resize_sub_nodes(std::size_t const);
    std::optional<ui::size> _waveform_scale() const;
};
}  // namespace yas::ae
