//
//  ae_track_content.cpp
//

#include "ae_track_content.hpp"

using namespace yas;
using namespace yas::ae;

namespace yas::ae::track_content_constants {
static float constexpr square_height_rate = 0.95f;
}

track_index_t const &track_content::track() const {
    return this->identifier;
}

std::string const track_content::name() const {
    return std::to_string(this->track());
}

ui::range track_content::vertical_range() const {
    return {.location = this->bottom_y(), .length = this->height()};
}

float const track_content::bottom_y() const {
    return (this->track() - (track_content_constants::square_height_rate * 0.5f)) * this->scale;
}

float const track_content::height() const {
    return this->scale * track_content_constants::square_height_rate;
}

bool track_content::operator==(track_content const &rhs) const {
    return this->identifier == rhs.identifier && this->is_selected == rhs.is_selected && this->scale == rhs.scale;
}

bool track_content::operator!=(track_content const &rhs) const {
    return !(*this == rhs);
}
