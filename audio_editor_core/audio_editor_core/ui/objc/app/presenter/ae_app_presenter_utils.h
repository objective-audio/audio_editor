//
//  ae_app_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_event.h>
#include <audio_editor_core/ae_app_settings_lifecycle_event.h>
#include <audio_editor_core/ae_window_lifecycle_event.h>

namespace yas::ae {
static app_presenter_window_event_type to_presenter_event_type(window_lifecycle_event_type const &type) {
    switch (type) {
        case window_lifecycle_event_type::inserted:
            return app_presenter_window_event_type::make_and_show_window_controller;
        case window_lifecycle_event_type::erased:
            return app_presenter_window_event_type::dispose_window_controller;
    }
}

static app_presenter_window_event_type to_presenter_event_type(project_settings_lifecycle_event_type const &type) {
    switch (type) {
        case project_settings_lifecycle_event_type::inserted:
            return app_presenter_window_event_type::make_and_show_project_settings;
        case project_settings_lifecycle_event_type::erased:
            return app_presenter_window_event_type::dispose_project_settings;
    }
}

static app_presenter_window_event_type to_presenter_event_type(app_settings_lifecycle_event_type const &type) {
    switch (type) {
        case app_settings_lifecycle_event_type::inserted:
            return app_presenter_window_event_type::make_and_show_app_settings;
        case app_settings_lifecycle_event_type::erased:
            return app_presenter_window_event_type::dispose_app_settings;
    }
}
}  // namespace yas::ae
