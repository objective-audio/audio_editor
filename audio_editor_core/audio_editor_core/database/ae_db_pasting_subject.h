//
//  ae_db_pasting_subject.h
//

#pragma once

#include <db/yas_db_umbrella.h>

namespace yas::ae {
struct db_pasting_subject final {
    explicit db_pasting_subject(db::object_ptr const &);

    [[nodiscard]] static db_pasting_subject create(db::manager_ptr const &, std::string const &data);

    std::string data() const;

    void remove();

   private:
    db::object_ptr const _object;
};
}  // namespace yas::ae
