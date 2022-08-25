//
//  ae_marker_name_editor.cpp
//

#include "ae_marker_name_editor.hpp"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>

using namespace yas;
using namespace yas::ae;

static std::string const empty_string = "";

std::shared_ptr<marker_name_editor> marker_name_editor::make_shared(sheet_lifetime_id const &lifetime_id,
                                                                    int64_t const marker_frame) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id.window);
    return std::make_shared<marker_name_editor>(marker_frame, project_lifetime->marker_pool.get());
}

marker_name_editor::marker_name_editor(int64_t const marker_frame, marker_pool *marker_pool)
    : _marker_frame(marker_frame), _marker_pool(marker_pool) {
}

std::string const &marker_name_editor::name() const {
    if (auto const marker = this->_marker_pool->marker_for_frame(this->_marker_frame)) {
        return marker->name;
    }
    return empty_string;
}

void marker_name_editor::set_name(std::string const &name) {
    if (auto opt_marker = this->_marker_pool->marker_for_frame(this->_marker_frame)) {
        auto &marker = opt_marker.value();
        marker.name = name;
        this->_marker_pool->replace_marker(marker);
    }
}
