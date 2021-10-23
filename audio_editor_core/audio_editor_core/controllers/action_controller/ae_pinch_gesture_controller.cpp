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
    return std::shared_ptr<pinch_gesture_controller>(new pinch_gesture_controller{project});
}

pinch_gesture_controller::pinch_gesture_controller(std::shared_ptr<project> const &project) : _project(project) {
}

void pinch_gesture_controller::handle_gesture(pinch_gesture const &gesture) {
    if (auto const project = this->_project.lock()) {
        switch (gesture.state) {
            case gesture_state::began:
                project->zooming()->begin();
                project->zooming()->set_magnification(gesture.magnification);
                break;
            case gesture_state::changed:
                project->zooming()->set_magnification(gesture.magnification);
                break;
            case gesture_state::ended:
                project->zooming()->set_magnification(gesture.magnification);
                project->zooming()->end();
                break;
        }
    }
}
