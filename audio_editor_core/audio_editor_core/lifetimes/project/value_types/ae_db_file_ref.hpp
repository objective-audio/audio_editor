//
//  ae_db_file_ref.hpp
//

#pragma once

#include <db/yas_db_umbrella.h>

#include <audio_editor_core/ae_file_ref.hpp>

namespace yas::ae {
struct db_file_ref final {
    explicit db_file_ref(db::object_ptr const &);

    [[nodiscard]] static db_file_ref create(db::manager_ptr const &, file_ref::params const &);

    [[nodiscard]] std::optional<file_ref> file_ref() const;

    void remove();

   private:
    db::object_ptr const _object;
};
}  // namespace yas::ae
