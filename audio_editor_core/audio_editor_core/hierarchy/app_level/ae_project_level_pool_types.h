//
//  ae_project_level_pool_types.h
//

#pragma once

#include <string>

namespace yas::ae {
enum class project_level_pool_event_type {
    inserted,
    erased,
};

struct project_level_pool_event final {
    project_level_pool_event_type type;
    std::string project_id;
};
}  // namespace yas::ae
