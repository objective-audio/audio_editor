//
//  ae_app_lifecycle.cpp
//

#include "ae_app_lifecycle.h"

#include <audio_editor_core/ae_action_id.h>
#include <audio_editor_core/ae_app_launcher.h>
#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

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

#pragma mark - action_receiver_provider

std::optional<action_id> app_lifecycle::receivable_id() const {
    return std::nullopt;
}

std::vector<action_receivable *> app_lifecycle::receivers() const {
    return {};
}

std::vector<action_receiver_providable *> app_lifecycle::sub_providers() const {
    return {this->_lifetime->project_lifecycle.get()};
}
