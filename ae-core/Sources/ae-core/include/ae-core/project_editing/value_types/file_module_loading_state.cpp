//
//  file_module_loading_state.cpp
//

#include "file_module_loading_state.h"

std::string yas::to_string(ae::file_module_loading_state const &state) {
    switch (state) {
        case ae::file_module_loading_state::waiting:
            return "waiting";
        case ae::file_module_loading_state::loading:
            return "loading";
    }
}
