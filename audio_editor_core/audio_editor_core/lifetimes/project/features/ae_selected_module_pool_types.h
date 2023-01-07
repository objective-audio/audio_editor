//
//  ae_selected_module_pool_types.h
//

#pragma once

#include <audio_editor_core/ae_module_index.hpp>
#include <audio_editor_core/ae_selected_module.hpp>
#include <map>

namespace yas::ae {
using selected_module_map = std::map<module_index, selected_module_object>;
}
