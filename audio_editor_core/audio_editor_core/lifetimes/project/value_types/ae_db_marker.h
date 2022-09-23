//
//  ae_db_marker.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <db/yas_db_umbrella.h>

namespace yas::ae {
class marker_object;

struct db_marker final {
    db_marker(db::object_ptr const &);

    [[nodiscard]] static db_marker create(db::manager_ptr const &, frame_index_t const frame, std::string const &name);

    void set_object(marker_object const &);
    std::optional<marker_object> object() const;

    void remove();

   private:
    db::object_ptr const _object;
};
}  // namespace yas::ae
