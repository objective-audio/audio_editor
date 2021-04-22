//
//  ae_app_presenter_types.h
//

#pragma once

#include <cstdint>

namespace yas::ae {
enum class app_presenter_event_type {
    make_and_show_window_controller,
    dispose_window_controller,
};

struct app_presenter_event {
    app_presenter_event_type type;
    uintptr_t project_id;
};
}  // namespace yas::ae
