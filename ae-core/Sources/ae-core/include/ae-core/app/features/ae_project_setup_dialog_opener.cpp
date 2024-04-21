//
//  ae_project_setup_dialog_opener.cpp
//

#include "ae_project_setup_dialog_opener.hpp"

#include <ae-core/app/lifecycles/ae_app_modal_lifecycle.h>

using namespace yas;
using namespace yas::ae;

project_setup_dialog_opener::project_setup_dialog_opener(app_modal_lifecycle *app_modal_lifecycle)
    : _app_modal_lifecycle(app_modal_lifecycle) {
}

bool project_setup_dialog_opener::can_open() const {
    return !this->_app_modal_lifecycle->current().has_value();
}

void project_setup_dialog_opener::open() {
    if (this->can_open()) {
        this->_app_modal_lifecycle->add_project_setup_dialog();
    }
}
