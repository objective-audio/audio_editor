//
//  ae_pasting_module_index.cpp
//

#include "ae_pasting_module_index.hpp"

#include <audio_editor_core/ae_space_range.hpp>

using namespace yas;
using namespace yas::ae;

pasting_module_index::pasting_module_index(yas::identifier const &identifier, time::range const &range,
                                           track_index_t const track)
    : identifier(identifier), range(range), track(track) {
}

bool pasting_module_index::operator==(pasting_module_index const &rhs) const {
    return this->identifier == rhs.identifier;
}

bool pasting_module_index::operator!=(pasting_module_index const &rhs) const {
    return !(*this == rhs);
}

bool pasting_module_index::operator<(pasting_module_index const &rhs) const {
    if (this->range != rhs.range) {
        return this->range < rhs.range;
    }

    if (this->track != rhs.track) {
        return this->track < rhs.track;
    }

    return this->identifier < rhs.identifier;
}

bool pasting_module_index::is_overlap(space_range const &space_range) const {
    return this->range.is_overlap(space_range.time_range) && space_range.track_range.contains(this->track);
}
