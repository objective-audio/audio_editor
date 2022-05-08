//
//  ae_project_level_router_types.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>

#include <string>

namespace yas::ae {
enum class project_level_router_event_type {
    inserted,
    erased,
};

struct project_level_router_event final {
    project_level_router_event_type type;
    project_id project_id;
};
}  // namespace yas::ae
