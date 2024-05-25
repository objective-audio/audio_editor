//
//  module_waveforms_presenter.cpp
//

#include "module_waveforms_presenter.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/project_path.h>
#include <ae-core/project_editing/features/waveform_mesh_importer.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<module_waveforms_presenter> module_waveforms_presenter::make_shared(
    project_lifetime_id const &lifetime_id) {
    auto const &lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id);
    return std::make_shared<module_waveforms_presenter>(lifetime->module_content_pool,
                                                        lifetime->waveforms_mesh_importer);
}

module_waveforms_presenter::module_waveforms_presenter(std::shared_ptr<module_content_pool> const &content_pool,
                                                       std::shared_ptr<waveform_mesh_importer> const &importer)
    : _mesh_importer(importer), _content_pool(content_pool) {
}

void module_waveforms_presenter::import(std::size_t const idx, module_content const &content) {
    if (auto const importer = this->_mesh_importer.lock()) {
        importer->import(idx, content);
    }
}

void module_waveforms_presenter::cancel_import(object_id const &identifier) {
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

std::vector<std::optional<module_content>> const &module_waveforms_presenter::contents() const {
    if (auto const content_pool = this->_content_pool.lock()) {
        return content_pool->elements();
    } else {
        static std::vector<std::optional<module_content>> const _empty;
        return _empty;
    }
}

observing::syncable module_waveforms_presenter::observe_contents(
    std::function<void(module_content_pool_event const &)> &&handler) {
    if (auto const content_pool = this->_content_pool.lock()) {
        return content_pool->observe_event(std::move(handler));
    } else {
        return observing::syncable{};
    }
}
