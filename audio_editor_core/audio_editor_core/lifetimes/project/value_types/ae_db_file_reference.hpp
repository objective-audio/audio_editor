//
//  ae_db_file_reference.hpp
//

#pragma once

#include <db/yas_db_umbrella.h>

namespace yas::ae {
class file_reference;

struct db_file_reference final {
    explicit db_file_reference(db::object_ptr const &);

    [[nodiscard]] static db_file_reference create(db::manager_ptr const &, file_reference const &);

    [[nodiscard]] std::optional<file_reference> file_reference() const;

    void remove();

   private:
    db::object_ptr const _object;
};
}  // namespace yas::ae
