//
//  ae_app_presenter_types.h
//

#pragma once

#include <string>

namespace yas::ae {
enum class app_presenter_event_type {
    open_file_dialog,
    make_and_show_window_controller,
    dispose_window_controller,
};

struct app_presenter_event {
    app_presenter_event_type type;
    std::string project_id;
};
}  // namespace yas::ae
