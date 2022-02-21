//
//  ae_context_menu.cpp
//

#include "ae_context_menu.h"

using namespace yas;
using namespace yas::ae;

bool context_menu::operator==(context_menu const &rhs) const {
    return this->location_in_view == rhs.location_in_view && this->actions == rhs.actions;
}

bool context_menu::operator!=(context_menu const &rhs) const {
    return !(*this == rhs);
}
