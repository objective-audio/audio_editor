//
//  ae_pinch_gesture_controller.cpp
//

#include "ae_pinch_gesture_controller.h"

#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_zooming_pair.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<pinch_gesture_controller> pinch_gesture_controller::make_shared(project_id const &project_id) {
    auto const project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<pinch_gesture_controller>(new pinch_gesture_controller{project_level->zooming_pair});
}

pinch_gesture_controller::pinch_gesture_controller(std::shared_ptr<zooming_pair> const &zooming_pair)
    : _zooming_pair(zooming_pair) {
}

void pinch_gesture_controller::handle_gesture(pinch_gesture const &gesture) {
    auto const zooming_pair = this->_zooming_pair.lock();
    if (!zooming_pair) {
        return;
    }

    auto const &zooming = this->_is_modified ? zooming_pair->vertical : zooming_pair->horizontal;

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

void pinch_gesture_controller::handle_modifier(modifier_event_state const &state) {
    switch (state) {
        case modifier_event_state::began:
            this->_is_modified = true;
            break;
        case modifier_event_state::ended:
            this->_is_modified = false;
            break;
    }

    if (auto const zooming_pair = this->_zooming_pair.lock()) {
        zooming_pair->horizontal->end();
        zooming_pair->vertical->end();
    }
}
