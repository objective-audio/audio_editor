//
//  ae_ui_base_lifecycle.cpp
//

#include "ae_ui_base_lifecycle.hpp"
#include <cpp_utils/yas_assertion.h>
#include <audio_editor_core/ae_ui_base_lifetime.hpp>

using namespace yas;
using namespace yas::ae;

ui_base_lifecycle::ui_base_lifecycle(ae::project_lifetime_id const &lifetime_id) : project_lifetime_id(lifetime_id) {
}

void ui_base_lifecycle::add_lifetime() {
    if (this->_lifetime) {
        assertion_failure_if_not_test();
        return;
    }

    this->_lifetime = std::make_shared<ui_base_lifetime>(this->project_lifetime_id);
}

std::shared_ptr<ui_base_lifetime> const &ui_base_lifecycle::lifetime() const {
    return this->_lifetime;
}
