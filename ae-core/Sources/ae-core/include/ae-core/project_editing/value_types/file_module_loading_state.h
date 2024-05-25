//
//  file_module_loading_state.h
//

#pragma once

#include <string>

namespace yas::ae {
enum class file_module_loading_state {
    waiting,
    loading,
};
}

namespace yas {
std::string to_string(ae::file_module_loading_state const &state);
}  // namespace yas
