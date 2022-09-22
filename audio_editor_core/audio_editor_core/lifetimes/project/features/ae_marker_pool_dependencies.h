//
//  ae_marker_pool_dependencies.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

namespace yas::ae {
class db_marker;
class marker_object;

struct database_for_marker_pool {
    virtual ~database_for_marker_pool() = default;

    [[nodiscard]] virtual db_marker add_marker(frame_index_t const frame, std::string const &name) = 0;
    virtual void remove_marker(frame_index_t const &) = 0;
    virtual void update_marker(frame_index_t const &prev_frame, marker_object const &) = 0;
};
}  // namespace yas::ae
