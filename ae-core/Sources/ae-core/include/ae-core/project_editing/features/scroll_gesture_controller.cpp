//
//  scroll_gesture_controller.cpp
//

#include "scroll_gesture_controller.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/global/value_types/gesture.h>
#include <ae-core/project/features/horizontal_scrolling.h>
#include <ae-core/project/features/zooming_pair.h>
#include <ae-core/ui_base/features/zooming/ui_zooming_constants.h>

#include <ae-core/project/features/vertical_scrolling.hpp>

using namespace yas;
using namespace yas::ae;

scroll_gesture_controller::scroll_gesture_controller(horizontal_scrolling_for_gesture_controller *scrolling,
                                                     vertical_scrolling *vertical_scrolling,
                                                     zooming_pair const *zooming_pair)
    : _horizontal_scrolling(scrolling), _vertical_scrolling(vertical_scrolling), _zooming_pair(zooming_pair) {
}

void scroll_gesture_controller::handle_gesture(scroll_gesture const &gesture) {
    auto const zooming_size = to_zooming_size(this->_zooming_pair->scale());
    auto const delta_time = -gesture.delta_x / zooming_size.width_per_sec;
    auto const delta_track = gesture.delta_y / zooming_size.height_per_track;

    switch (gesture.state) {
        case gesture_state::began:
            this->_horizontal_scrolling->begin();
            this->_vertical_scrolling->begin();

            this->_horizontal_scrolling->set_delta_time(delta_time);
            this->_vertical_scrolling->set_delta_position(delta_track);
            break;

        case gesture_state::changed:
            this->_horizontal_scrolling->set_delta_time(delta_time);
            this->_vertical_scrolling->set_delta_position(delta_track);
            break;

        case gesture_state::ended:
            this->_horizontal_scrolling->set_delta_time(delta_time);
            this->_vertical_scrolling->set_delta_position(delta_track);

            this->_horizontal_scrolling->end();
            this->_vertical_scrolling->end();
            break;
    }
}
