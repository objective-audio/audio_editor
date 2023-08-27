//
//  ae_selected_marker_pool.hpp
//

#pragma once

#include <audio_editor_core/ae_marker_index.hpp>
#include <audio_editor_core/ae_selected_marker.hpp>
#include <audio_editor_core/ae_selected_pool.hpp>

namespace yas::ae {
using selected_marker_pool = selected_pool<marker_index>;
using selected_marker_set = selected_marker_pool::element_set;
}  // namespace yas::ae
