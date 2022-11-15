//
//  ae_app_presenter_types.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

#include <string>

namespace yas::ae {
enum class app_presenter_window_event_type {
    make_and_show_window_controller,
    dispose_window_controller,
};

struct app_presenter_window_event final {
    app_presenter_window_event_type type;
    window_lifetime_id lifetime_id;
};
}  // namespace yas::ae
