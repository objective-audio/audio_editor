//
//  display_space_time_range.mm
//

#include "display_space_time_range.hpp"

#include <ae-core/project/features/display_space.h>
#include <ae-core/project/features/player.h>

using namespace yas;
using namespace yas::ae;

display_space_time_range::display_space_time_range(project_format const project_format,
                                                   display_space const *display_space, player const *player)
    : _project_format(project_format), _display_space(display_space), _player(player) {
}

std::optional<time::range> display_space_time_range::current() const {
    return this->_range(this->_player->current_frame());
}

std::optional<time::range> display_space_time_range::zero() const {
    return this->_range(0);
}

std::optional<time::range> display_space_time_range::_range(frame_index_t const frame) const {
    return this->_display_space->frame_range(this->_project_format.sample_rate, frame);
}
