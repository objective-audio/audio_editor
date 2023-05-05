//
//  ae_db_module.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_module.h>
#include <db/yas_db_umbrella.h>

namespace yas::ae {
class module_object;

struct db_module final {
    explicit db_module(db::object_ptr const &);

    [[nodiscard]] static db_module create(db::manager_ptr const &, module const &);

    [[nodiscard]] object_id const &object_id() const;
    [[nodiscard]] std::optional<module_object> object() const;

    void set_range(time::range const &);
    void set_name(std::string const &);
    void remove();

   private:
    db::object_ptr const _object;
};
}  // namespace yas::ae
