//
//  ae_module_waveforms_presenter.cpp
//

#include "ae_module_waveforms_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_path.h>
#include <audio_editor_core/ae_waveform_mesh_importer.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<module_waveforms_presenter> module_waveforms_presenter::make_shared(
    window_lifetime_id const &lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id);
    return std::make_shared<module_waveforms_presenter>(project_lifetime->module_location_pool,
                                                        project_lifetime->waveforms_mesh_importer);
}

module_waveforms_presenter::module_waveforms_presenter(std::shared_ptr<module_location_pool> const &location_pool,
                                                       std::shared_ptr<waveform_mesh_importer> const &importer)
    : _mesh_importer(importer), _location_pool(location_pool) {
}

void module_waveforms_presenter::import(std::size_t const idx, module_location const &location) {
    if (auto const importer = this->_mesh_importer.lock()) {
        importer->import(idx, location);
    }
}

void module_waveforms_presenter::cancel_import(identifier const &identifier) {
    if (auto const importer = this->_mesh_importer.lock()) {
        importer->cancel(identifier);
    }
}

observing::endable module_waveforms_presenter::observe_mesh_importer(
    std::function<void(waveform_mesh_importer_event const &)> &&handler) {
    if (auto const importer = this->_mesh_importer.lock()) {
        return importer->observe(std::move(handler));
    } else {
        return observing::endable{};
    }
}

std::vector<std::optional<module_location>> const &module_waveforms_presenter::locations() const {
    if (auto const location_pool = this->_location_pool.lock()) {
        return location_pool->elements();
    } else {
        static std::vector<std::optional<module_location>> const _empty;
        return _empty;
    }
}

observing::syncable module_waveforms_presenter::observe_locations(
    std::function<void(module_location_pool_event const &)> &&handler) {
    if (auto const location_pool = this->_location_pool.lock()) {
        return location_pool->observe_event(std::move(handler));
    } else {
        return observing::syncable{};
    }
}
