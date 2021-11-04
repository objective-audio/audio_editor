//
//  ae_db_marker.h
//

#pragma once

#include <db/yas_db_umbrella.h>

namespace yas::ae {
class marker;

struct db_marker {
    db_marker(db::object_ptr const &);

    [[nodiscard]] static db_marker create(db::manager_ptr const &, marker const &);

    std::optional<marker> marker() const;

    void remove();

   private:
    db::object_ptr const _object;
};
}  // namespace yas::ae
