//
//  ae_project_pool_types.h
//

#pragma once

#include <cstdint>

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
