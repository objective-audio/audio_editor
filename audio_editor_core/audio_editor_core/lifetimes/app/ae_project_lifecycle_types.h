//
//  ae_project_lifecycle_types.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>

#include <string>

namespace yas::ae {
enum class project_lifecycle_event_type {
    inserted,
    erased,
};

struct project_lifecycle_event final {
    project_lifecycle_event_type type;
    project_id project_id;
};
}  // namespace yas::ae
