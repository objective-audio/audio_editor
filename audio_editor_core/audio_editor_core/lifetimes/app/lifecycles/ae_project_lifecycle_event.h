//
//  ae_project_lifecycle_event.h
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>

namespace yas::ae {
enum class project_lifecycle_event_type {
    inserted,
    erased,
};

struct project_lifecycle_event final {
    project_lifecycle_event_type type;
    project_lifetime_id lifetime_id;
};
}  // namespace yas::ae
