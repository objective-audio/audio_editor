//
//  module_pool_types.h
//

#pragma once

#include <ae-core/project_editing/value_types/module.h>

#include <map>

namespace yas::ae {
using module_pool_module_map_t = std::map<module_index, module_object>;

enum class module_pool_event_type {
    any,
    reverted,
    inserted,
    erased,
    detail_updated,
};

struct module_pool_event final {
    module_pool_event_type type;
    std::optional<module_object> module{std::nullopt};  // inserted, erased
    module_pool_module_map_t const &modules;
};
}  // namespace yas::ae
