//
//  ae_pinch_gesture_controller.cpp
//

#include "ae_pinch_gesture_controller.h"

#include <ae-core/global/ae_hierarchy.h>
#include <ae-core/global/value_types/ae_gesture.h>
#include <ae-core/project/features/ae_zooming_pair.h>

using namespace yas;
using namespace yas::ae;

pinch_gesture_controller::pinch_gesture_controller(zooming_pair *zooming_pair) : _zooming_pair(zooming_pair) {
}

void pinch_gesture_controller::handle_gesture(pinch_gesture const &gesture) {
    auto const &zooming = this->_is_modified ? this->_zooming_pair->vertical : this->_zooming_pair->horizontal;

    switch (gesture.state) {
        case gesture_state::began:
            zooming->begin();
            zooming->set_magnification(gesture.magnification);
            break;
        case gesture_state::changed:
            zooming->set_magnification(gesture.magnification);
            break;
        case gesture_state::ended:
            zooming->set_magnification(gesture.magnification);
            zooming->end();
            break;
    }
}

void pinch_gesture_controller::handle_modifier(modifiers_holder_event_state const &state) {
    switch (state) {
        case modifiers_holder_event_state::began:
            this->_is_modified = true;
            break;
        case modifiers_holder_event_state::ended:
            this->_is_modified = false;
            break;
    }

    this->_zooming_pair->horizontal->end();
    this->_zooming_pair->vertical->end();
}
