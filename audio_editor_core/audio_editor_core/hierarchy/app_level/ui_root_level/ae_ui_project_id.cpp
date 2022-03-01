//
//  ae_ui_project_id.cpp
//

#include "ae_ui_project_id.h"

using namespace yas;
using namespace yas::ae;

bool ui_project_id::operator==(ui_project_id const &rhs) {
    return this->identifier == rhs.identifier && this->view_id == rhs.view_id;
}

bool ui_project_id::operator!=(ui_project_id const &rhs) {
    return !(*this == rhs);
}
