//
//  ae_pinch_gesture_controller.cpp
//

#include "ae_pinch_gesture_controller.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_project_pool.h>
#include <audio_editor_core/ae_zooming.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<pinch_gesture_controller> pinch_gesture_controller::make_shared(std::string const &project_id) {
    auto const project = app_level::global()->project_pool->project_level_for_id(project_id)->project;
    return std::shared_ptr<pinch_gesture_controller>(
        new pinch_gesture_controller{project->horizontal_zooming, project->vertical_zooming});
}

pinch_gesture_controller::pinch_gesture_controller(
    std::shared_ptr<zooming_for_pinch_gesture_controller> const &horizontal_zooming,
    std::shared_ptr<zooming_for_pinch_gesture_controller> const &vertical_zooming)
    : _horizontal_zooming(horizontal_zooming), _vertical_zooming(vertical_zooming) {
}

void pinch_gesture_controller::handle_gesture(pinch_gesture const &gesture) {
    auto const &current_zooming = this->_is_modified ? this->_vertical_zooming : this->_horizontal_zooming;

    if (auto const zooming = current_zooming.lock()) {
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

    if (auto const zooming = this->_horizontal_zooming.lock()) {
        zooming->end();
    }

    if (auto const zooming = this->_vertical_zooming.lock()) {
        zooming->end();
    }
}
