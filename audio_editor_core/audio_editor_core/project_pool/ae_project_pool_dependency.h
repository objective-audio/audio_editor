//
//  ae_project_pool_dependency.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
enum class project_pool_event_type {
    inserted,
    erased,
};

struct project_pool_event {
    project_pool_event_type type;
    uintptr_t project_id;
};
}  // namespace yas::ae
