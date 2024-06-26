//
//  selected_track_pool.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

#include <ae-core/global/features/selected_pool.hpp>

namespace yas::ae {
using selected_track_pool = selected_pool<track_index_t>;
using selected_track_set = selected_track_pool::element_set;
}  // namespace yas::ae
