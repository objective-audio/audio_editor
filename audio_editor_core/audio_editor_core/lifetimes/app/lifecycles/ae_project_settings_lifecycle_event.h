//
//  ae_project_settings_lifecycle_event.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
enum class project_settings_lifecycle_event_type {
    inserted,
    erased,
};

struct project_settings_lifecycle_event final {
    project_settings_lifecycle_event_type type;
    window_lifetime_id lifetime_id;
};
}  // namespace yas::ae
