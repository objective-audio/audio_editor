//
//  ae_selected_file_module_pool_types.h
//

#pragma once

#include <audio_editor_core/ae_file_module_index.hpp>
#include <audio_editor_core/ae_selected_file_module.hpp>
#include <map>

namespace yas::ae {
using selected_file_module_map = std::map<file_module_index, selected_file_module_object>;
}
