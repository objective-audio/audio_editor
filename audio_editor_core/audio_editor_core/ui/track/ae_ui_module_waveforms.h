//
//  ae_ui_module_waveforms.h
//

#pragma once

#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_ui_project_id.h>
#include <cpp_utils/yas_url.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class module_location;
class module_waveforms_presenter;
class color;

struct ui_module_waveforms final {
    [[nodiscard]] static std::shared_ptr<ui_module_waveforms> make_shared(
        ui_project_id const &project_id, std::shared_ptr<module_location_pool> const &);

    std::shared_ptr<ui::node> const node;

    void set_scale(ui::size const &);

    void set_locations(std::vector<std::optional<module_location>> const &, bool const clear_mesh_nodes);
    void update_locations(std::size_t const count, std::vector<std::pair<std::size_t, module_location>> const &erased,
                          std::vector<std::pair<std::size_t, module_location>> const &inserted,
                          std::vector<std::pair<std::size_t, module_location>> const &replaced);

   private:
    std::shared_ptr<module_waveforms_presenter> const _presenter;
    std::shared_ptr<ae::color> const _color;
    std::optional<float> _scale = std::nullopt;

    observing::canceller_pool _pool;

    ui_module_waveforms(std::shared_ptr<ui::standard> const &, std::shared_ptr<ae::color> const &,
                        std::shared_ptr<module_waveforms_presenter> const &);

    void _resize_sub_nodes(std::size_t const);
    std::optional<ui::size> _waveform_scale() const;
};
}  // namespace yas::ae
