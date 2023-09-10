//
//  ae_selected_track_pool.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <audio_editor_core/ae_selected_pool.hpp>

namespace yas::ae {
using selected_track_pool = selected_pool<track_index_t>;
using selected_track_set = selected_track_pool::element_set;
}  // namespace yas::ae
