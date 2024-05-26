//
//  action_utils.cpp
//

#include "action_utils.h"

using namespace yas;
using namespace yas::ae;

std::string action_utils::to_context_menu_title(ae::action const &action) {
    switch (to_kind(action.name)) {
        default:
            return "";
    }
}
