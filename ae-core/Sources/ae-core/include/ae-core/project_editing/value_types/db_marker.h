//
//  db_marker.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

#include <db/umbrella.hpp>

namespace yas::ae {
class marker_object;

struct db_marker final {
    db_marker(db::object_ptr const &);

    [[nodiscard]] static db_marker create(db::manager_ptr const &, frame_index_t const frame, std::string const &name);

    [[nodiscard]] object_id const &object_id() const;

    std::optional<marker_object> object() const;
    void set_object(marker_object const &);

    void remove();

   private:
    db::object_ptr const _object;
};
}  // namespace yas::ae
