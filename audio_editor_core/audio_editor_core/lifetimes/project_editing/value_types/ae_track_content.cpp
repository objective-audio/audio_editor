//
//  ae_track_content.cpp
//

#include "ae_track_content.hpp"

using namespace yas;
using namespace yas::ae;

track_index_t const &track_content::track() const {
    return this->identifier;
}

bool track_content::operator==(track_content const &rhs) const {
    return this->identifier == rhs.identifier && this->is_selected == rhs.is_selected;
}

bool track_content::operator!=(track_content const &rhs) const {
    return !(*this == rhs);
}
