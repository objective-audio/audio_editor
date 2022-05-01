//
//  ae_module_waveforms_presenter.cpp
//

#include "ae_module_waveforms_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_waveform_mesh_importer.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<module_waveforms_presenter> module_waveforms_presenter::make_shared(
    std::string const &project_id, std::shared_ptr<module_location_pool> const &location_pool) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    auto const url = project_level->project_url->editing_file();
    return std::shared_ptr<module_waveforms_presenter>(new module_waveforms_presenter{url, location_pool});
}

module_waveforms_presenter::module_waveforms_presenter(url const &url,
                                                       std::shared_ptr<module_location_pool> const &location_pool)
    : _mesh_importer(waveform_mesh_importer::make_shared(url)), _location_pool(location_pool) {
}

void module_waveforms_presenter::import(std::size_t const idx, module_location const &location) {
    this->_mesh_importer->import(idx, location);
}

void module_waveforms_presenter::cancel_import(identifier const &identifier) {
    this->_mesh_importer->cancel(identifier);
}

observing::endable module_waveforms_presenter::observe_mesh_importer(
    std::function<void(waveform_mesh_importer_event const &)> &&handler) {
    return this->_mesh_importer->observe(std::move(handler));
}

std::vector<std::optional<module_location>> const &module_waveforms_presenter::locations() const {
    return this->_location_pool->elements();
}

observing::syncable module_waveforms_presenter::observe_locations(
    std::function<void(module_location_pool_event const &)> &&handler) {
    return this->_location_pool->observe_event(std::move(handler));
}
