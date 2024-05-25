//
//  module_pool_dependencies.h
//

#pragma once

#include <ae-core/project_editing/value_types/module.h>

namespace yas::ae {
class db_module;

struct database_for_module_pool {
    virtual ~database_for_module_pool() = default;

    [[nodiscard]] virtual db_module add_module(module const &) = 0;
    virtual void remove_module(object_id const &) = 0;
    virtual void update_module(object_id const &, module const &) = 0;
};
}  // namespace yas::ae
