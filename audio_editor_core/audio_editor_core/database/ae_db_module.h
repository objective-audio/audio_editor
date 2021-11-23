//
//  ae_db_module.h
//

#pragma once

#include <db/yas_db_umbrella.h>

namespace yas::ae {
class file_module;

struct db_module final {
    explicit db_module(db::object_ptr const &);

    [[nodiscard]] static db_module create(db::manager_ptr const &, file_module const &);

    [[nodiscard]] std::optional<file_module> file_module() const;

    void remove();

   private:
    db::object_ptr const _object;
};
}  // namespace yas::ae
