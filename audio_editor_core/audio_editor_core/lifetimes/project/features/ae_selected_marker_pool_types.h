//
//  ae_selected_marker_pool_types.h
//

#pragma once

#include <audio_editor_core/ae_marker_index.hpp>
#include <audio_editor_core/ae_selected_marker.hpp>
#include <map>

namespace yas::ae {
using selected_marker_map = std::map<marker_index, selected_marker_object>;
}
