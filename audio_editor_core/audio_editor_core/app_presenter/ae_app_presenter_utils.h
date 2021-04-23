//
//  ae_app_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_types.h>
#include <audio_editor_core/ae_project_pool_types.h>

namespace yas::ae {
static app_presenter_event_type to_presenter_event_type(project_pool_event_type const &type) {
    switch (type) {
        case project_pool_event_type::inserted:
            return app_presenter_event_type::make_and_show_window_controller;
        case project_pool_event_type::erased:
            return app_presenter_event_type::dispose_window_controller;
    }
}
}  // namespace yas::ae
