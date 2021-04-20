//
//  ae_app_types.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>

namespace yas::ae {
enum class app_projects_event_type {
    inserted,
    erased,
};

struct app_projects_event {
    app_projects_event_type type;
    uintptr_t project_id;
};
}  // namespace yas::ae
