//
//  ae_scroll_gesture_controller.cpp
//

#include "ae_scroll_gesture_controller.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>
#include <audio_editor_core/ae_scrolling.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<scroll_gesture_controller> scroll_gesture_controller::make_shared(std::string const &project_id) {
    auto const project = app_level::global()->project_pool->project_for_id(project_id);
    return std::shared_ptr<scroll_gesture_controller>(new scroll_gesture_controller{project->scrolling()});
}

scroll_gesture_controller::scroll_gesture_controller(std::shared_ptr<scrolling_for_gesture_controller> const &scrolling)
    : _scrolling(scrolling) {
}

void scroll_gesture_controller::handle_gesture(scroll_gesture const &gesture) {
    if (auto const scrolling = this->_scrolling.lock()) {
        switch (gesture.state) {
            case gesture_state::began:
                scrolling->begin();
                scrolling->set_delta_time(gesture.delta_time);
                break;
            case gesture_state::changed:
                scrolling->set_delta_time(gesture.delta_time);
                break;
            case gesture_state::ended:
                scrolling->set_delta_time(gesture.delta_time);
                scrolling->end();
                break;
        }
    }
}
