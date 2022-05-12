//
//  ae_app_level_router.cpp
//

#include "ae_app_level_router.h"

#include <audio_editor_core/ae_app_launcher.h>
#include <audio_editor_core/ae_app_level.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_level_router> app_level_router::make_shared() {
    return std::make_shared<app_level_router>();
}

app_level_router::app_level_router() {
}

std::shared_ptr<app_level> const &app_level_router::level() const {
    return this->_level;
}

void app_level_router::add_level() {
    if (this->_level) {
        assertion_failure_if_not_test();
        return;
    }
    this->_level = app_level::make_shared();
    this->_level->launcher->launch();
}
