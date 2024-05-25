//
//  module_name_editor.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <ae-core/global/value_types/project_sub_lifetime_id.h>

#include <ae-core/project_editing/value_types/module_index.hpp>

namespace yas::ae {
class module_pool;
class database;

struct module_name_editor final {
    [[nodiscard]] static std::shared_ptr<module_name_editor> make_shared(project_sub_lifetime_id const &,
                                                                         module_index const &);
    module_name_editor(module_index const &, module_pool *, database *);

    [[nodiscard]] std::string const &name() const;
    void set_name(std::string const &);

   private:
    module_index const _module_index;
    module_pool *const _module_pool;
    database *const _database;

    module_name_editor(module_name_editor const &) = delete;
    module_name_editor(module_name_editor &&) = delete;
    module_name_editor &operator=(module_name_editor const &) = delete;
    module_name_editor &operator=(module_name_editor &&) = delete;
};
}  // namespace yas::ae
