//
//  ae_module_waveforms_presenter.h
//

#pragma once

#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_waveform_mesh_importer_types.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class waveform_mesh_importer;

struct module_waveforms_presenter final {
    [[nodiscard]] static std::shared_ptr<module_waveforms_presenter> make_shared(window_lifetime_id const &);

    module_waveforms_presenter(std::shared_ptr<module_location_pool> const &,
                               std::shared_ptr<waveform_mesh_importer> const &);

    void import(std::size_t const, module_location const &);
    void cancel_import(object_id const &);
    observing::endable observe_mesh_importer(std::function<void(waveform_mesh_importer_event const &)> &&);

    [[nodiscard]] std::vector<std::optional<module_location>> const &locations() const;
    [[nodiscard]] observing::syncable observe_locations(
        std::function<void(module_location_pool_event const &)> &&handler);

   private:
    std::weak_ptr<waveform_mesh_importer> const _mesh_importer;
    std::weak_ptr<module_location_pool> const _location_pool;
};
}  // namespace yas::ae
