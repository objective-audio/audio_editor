//
//  ae_module_index.cpp
//

#include "ae_module_index.hpp"

#include <ae-core/global/value_types/ae_space_range.hpp>

using namespace yas;
using namespace yas::ae;

module_index::module_index(ae::object_id const &object_id, time::range const &range, track_index_t const track)
    : object_id(object_id), range(range), track(track) {
}

bool module_index::operator==(module_index const &rhs) const {
    return this->object_id == rhs.object_id;
}

bool module_index::operator!=(module_index const &rhs) const {
    return !(*this == rhs);
}

bool module_index::operator<(module_index const &rhs) const {
    if (this->range != rhs.range) {
        return this->range < rhs.range;
    }

    if (this->track != rhs.track) {
        return this->track < rhs.track;
    }

    return this->object_id.identifier() < rhs.object_id.identifier();
}

bool module_index::is_overlap(space_range const &space_range) const {
    return this->range.is_overlap(space_range.time_range) && space_range.track_range.contains(this->track);
}
