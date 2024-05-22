//
//  ae_track_jumper.cpp
//

#include "ae_track_jumper.hpp"

#include <ae-core/project_editing/features/ae_module_pool.h>

#include <ae-core/project/features/ae_vertical_scrolling.hpp>

using namespace yas;
using namespace yas::ae;

track_jumper::track_jumper(module_pool const *module_pool, vertical_scrolling *vertical_scrolling)
    : _module_pool(module_pool), _vertical_scrolling(vertical_scrolling) {
}

bool track_jumper::can_jump_to_beginning() const {
    return !this->_module_pool->modules().empty();
}

bool track_jumper::can_jump_to_end() const {
    return !this->_module_pool->modules().empty();
}

void track_jumper::jump_to_beginning() {
    if (auto const first_track = this->_module_pool->first_track(); first_track.has_value()) {
        this->_vertical_scrolling->move_to_track(first_track.value());
    }
}

void track_jumper::jump_to_end() {
    if (auto const last_track = this->_module_pool->last_track(); last_track.has_value()) {
        this->_vertical_scrolling->move_to_track(last_track.value());
    }
}
