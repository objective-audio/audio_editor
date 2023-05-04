//
//  ae_app_presenter_event.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

#include <audio_editor_core/ae_app_settings_lifetime_id.hpp>
#include <variant>

namespace yas::ae {
enum class app_presenter_window_event_type {
    make_and_show_window_controller,  // window_lifetime_id
    dispose_window_controller,        // window_lifetime_id
    make_and_show_project_settings,   // window_lifetime_id
    dispose_project_settings,         // window_lifetime_id
    make_and_show_app_settings,       // app_settings_lifetime_id
    dispose_app_settings,             // app_settings_lifetime_id
};

using app_presenter_window_event_value = std::variant<window_lifetime_id, app_settings_lifetime_id>;

struct app_presenter_window_event final {
    app_presenter_window_event_type type;
    app_presenter_window_event_value value;

    window_lifetime_id const &window_lifetime_id() const {
        return std::get<ae::window_lifetime_id>(value);
    }

    app_settings_lifetime_id const &app_settings_lifetime_id() const {
        return std::get<ae::app_settings_lifetime_id>(value);
    }
};
}  // namespace yas::ae
