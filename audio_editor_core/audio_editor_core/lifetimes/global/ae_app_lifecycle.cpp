//
//  ae_app_lifecycle.cpp
//

#include "ae_app_lifecycle.h"

#include <audio_editor_core/ae_app_launcher.h>
#include <audio_editor_core/ae_app_lifetime.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_lifecycle> app_lifecycle::make_shared() {
    return std::make_shared<app_lifecycle>();
}

app_lifecycle::app_lifecycle() {
}

std::shared_ptr<app_lifetime> const &app_lifecycle::lifetime() const {
    return this->_lifetime;
}

void app_lifecycle::add() {
    if (this->_lifetime) {
        assertion_failure_if_not_test();
        return;
    }
    this->_lifetime = app_lifetime::make_shared();
    this->_lifetime->launcher->launch();
}
