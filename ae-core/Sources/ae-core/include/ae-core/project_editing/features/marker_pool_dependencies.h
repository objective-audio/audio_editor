//
//  marker_pool_dependencies.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

namespace yas::ae {
class db_marker;
class marker_object;

struct database_for_marker_pool {
    virtual ~database_for_marker_pool() = default;

    [[nodiscard]] virtual db_marker add_marker(frame_index_t const frame, std::string const &name) = 0;
    virtual void remove_marker(object_id const &) = 0;
    virtual void update_marker(object_id const &, marker_object const &) = 0;
};
}  // namespace yas::ae
