//
//  ae_app_settings_opener.cpp
//

#include "ae_app_settings_opener.hpp"

#include <audio_editor_core/ae_app_settings_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

app_settings_opener::app_settings_opener(app_settings_lifecycle *app_settings_lifecycle)
    : _app_settings_lifecycle(app_settings_lifecycle) {
}

bool app_settings_opener::can_open() const {
    return this->_app_settings_lifecycle->has_current();
}

void app_settings_opener::open() {
    if (this->can_open()) {
        this->_app_settings_lifecycle->add_lifetime();
    }
}
