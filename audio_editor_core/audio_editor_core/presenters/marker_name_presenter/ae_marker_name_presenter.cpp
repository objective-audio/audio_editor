//
//  ae_marker_name_presenter.cpp
//

#include "ae_marker_name_presenter.hpp"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<marker_name_presenter> marker_name_presenter::make_shared(sheet_lifetime_id const &lifetime_id,
                                                                          int64_t const marker_frame) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id.window);

    return std::make_shared<marker_name_presenter>(lifetime_id, marker_frame, project_lifetime->marker_pool,
                                                   project_lifetime->modal_lifecycle);
}

marker_name_presenter::marker_name_presenter(sheet_lifetime_id const &lifetime_id, int64_t const marker_frame,
                                             std::shared_ptr<marker_pool> const &marker_pool,
                                             std::shared_ptr<project_modal_lifecycle> const &lifecycle)
    : _lifetime_id(lifetime_id), _marker_frame(marker_frame), _marker_pool(marker_pool), _lifecycle(lifecycle) {
}

std::string marker_name_presenter::name() const {
    if (auto const marker_pool = this->_marker_pool.lock()) {
        if (auto const marker = marker_pool->marker_for_frame(this->_marker_frame)) {
            return marker->name;
        }
    }
    return "";
}

void marker_name_presenter::done(std::string const &name) {
    if (auto const marker_pool = this->_marker_pool.lock()) {
        if (auto opt_marker = marker_pool->marker_for_frame(this->_marker_frame)) {
            auto &marker = opt_marker.value();
            marker.name = name;
            marker_pool->insert_marker(marker);
        }
    }

    this->_finalize();
}

void marker_name_presenter::cancel() {
    this->_finalize();
}

void marker_name_presenter::_finalize() {
    if (auto const lifecycle = this->_lifecycle.lock()) {
        lifecycle->remove_sheet(this->_lifetime_id);
    }

    this->_marker_pool.reset();
    this->_lifecycle.reset();
}
