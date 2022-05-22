//
//  ae_scroll_gesture_controller.cpp
//

#include "ae_scroll_gesture_controller.h"

#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_scrolling.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<scroll_gesture_controller> scroll_gesture_controller::make_shared(
    scrolling_for_gesture_controller *scrolling) {
    return std::make_shared<scroll_gesture_controller>(scrolling);
}

scroll_gesture_controller::scroll_gesture_controller(scrolling_for_gesture_controller *scrolling)
    : _scrolling(scrolling) {
}

void scroll_gesture_controller::handle_gesture(scroll_gesture const &gesture) {
    switch (gesture.state) {
        case gesture_state::began:
            this->_scrolling->begin();
            this->_scrolling->set_delta_time(gesture.delta_time);
            break;
        case gesture_state::changed:
            this->_scrolling->set_delta_time(gesture.delta_time);
            break;
        case gesture_state::ended:
            this->_scrolling->set_delta_time(gesture.delta_time);
            this->_scrolling->end();
            break;
    }
}
