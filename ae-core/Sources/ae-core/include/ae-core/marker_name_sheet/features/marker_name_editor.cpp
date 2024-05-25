//
//  marker_name_editor.cpp
//

#include "marker_name_editor.hpp"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project_editing/features/marker_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<marker_name_editor> marker_name_editor::make_shared(project_sub_lifetime_id const &lifetime_id,
                                                                    marker_index const &index) {
    auto const &lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id.project);
    return std::make_shared<marker_name_editor>(index, lifetime->marker_pool.get());
}

marker_name_editor::marker_name_editor(marker_index const &index, marker_pool *marker_pool)
    : _marker_index(index), _marker_pool(marker_pool) {
}

std::string marker_name_editor::name() const {
    if (auto const marker = this->_marker_pool->marker_for_index(this->_marker_index)) {
        return marker->value.name;
    }
    return "";
}

void marker_name_editor::set_name(std::string const &name) {
    if (auto opt_marker = this->_marker_pool->marker_for_index(this->_marker_index)) {
        auto marker = opt_marker.value();
        marker.value.name = name;
        this->_marker_pool->update(this->_marker_index, marker);
    }
}
