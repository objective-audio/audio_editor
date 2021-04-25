//
//  ae_project_pool_types.h
//

#pragma once

#include <string>

namespace yas::ae {
enum class project_pool_event_type {
    inserted,
    erased,
};

struct project_pool_event {
    project_pool_event_type type;
    std::string project_id;
};
}  // namespace yas::ae
