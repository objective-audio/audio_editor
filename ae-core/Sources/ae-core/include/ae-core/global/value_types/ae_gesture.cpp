//
//  ae_gesture.cpp
//

#include "ae_gesture.h"

#include <ui/event/yas_ui_event_types.h>

using namespace yas;
using namespace yas::ae;

ae::gesture_state yas::to_gesture_state(ui::event_phase const phase) {
    switch (phase) {
        case ui::event_phase::began:
            return gesture_state::began;
        case ui::event_phase::changed:
        case ui::event_phase::stationary:
            return gesture_state::changed;
        case ui::event_phase::ended:
        case ui::event_phase::canceled:
        case ui::event_phase::may_begin:
        case ui::event_phase::none:
            return gesture_state::ended;
    }
}
