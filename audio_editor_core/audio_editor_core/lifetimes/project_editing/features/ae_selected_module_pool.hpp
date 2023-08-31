//
//  ae_selected_module_pool.hpp
//

#pragma once

#include <audio_editor_core/ae_module_index.hpp>
#include <audio_editor_core/ae_selected_module.hpp>
#include <audio_editor_core/ae_selected_pool.hpp>

namespace yas::ae {
using selected_module_pool = selected_pool<module_index>;
using selected_module_set = selected_module_pool::element_set;
}  // namespace yas::ae
