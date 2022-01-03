//
//  ae_pinch_gesture_controller.cpp
//

#include "ae_pinch_gesture_controller.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>
#include <audio_editor_core/ae_zooming.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<pinch_gesture_controller> pinch_gesture_controller::make_shared(std::string const &project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return std::shared_ptr<pinch_gesture_controller>(new pinch_gesture_controller{project->zooming()});
}

pinch_gesture_controller::pinch_gesture_controller(std::shared_ptr<zooming_for_pinch_gesture_controller> const &zooming)
    : _zooming(zooming) {
}

void pinch_gesture_controller::handle_gesture(pinch_gesture const &gesture) {
    if (auto const zooming = this->_zooming.lock()) {
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
}
