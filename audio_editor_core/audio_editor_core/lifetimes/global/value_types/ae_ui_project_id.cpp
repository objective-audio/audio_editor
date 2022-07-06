//
//  ae_ui_project_id.cpp
//

#include "ae_ui_project_id.h"

using namespace yas;
using namespace yas::ae;

bool ui_project_id::operator==(ui_project_id const &rhs) {
    return this->window_lifetime_id == rhs.window_lifetime_id && this->view_id == rhs.view_id;
}

bool ui_project_id::operator!=(ui_project_id const &rhs) {
    return !(*this == rhs);
}
