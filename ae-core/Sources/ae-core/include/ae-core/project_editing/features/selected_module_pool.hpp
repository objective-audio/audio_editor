//
//  selected_module_pool.hpp
//

#pragma once

#include <ae-core/global/features/selected_pool.hpp>
#include <ae-core/project_editing/value_types/module_index.hpp>
#include <ae-core/project_editing/value_types/selected_module.hpp>

namespace yas::ae {
using selected_module_pool = selected_pool<module_index>;
using selected_module_set = selected_module_pool::element_set;
}  // namespace yas::ae
