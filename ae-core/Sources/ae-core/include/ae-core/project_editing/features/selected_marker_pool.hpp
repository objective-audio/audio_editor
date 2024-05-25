//
//  selected_marker_pool.hpp
//

#pragma once

#include <ae-core/global/features/selected_pool.hpp>
#include <ae-core/project_editing/value_types/marker_index.hpp>
#include <ae-core/project_editing/value_types/selected_marker.hpp>

namespace yas::ae {
using selected_marker_pool = selected_pool<marker_index>;
using selected_marker_set = selected_marker_pool::element_set;
}  // namespace yas::ae
