//
//  ae_project_settings_opener.cpp
//

#include "ae_project_settings_opener.hpp"

#include <audio_editor_core/ae_project_settings_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

project_settings_opener::project_settings_opener(window_lifetime_id const lifetime_id,
                                                 project_settings_lifecycle *lifecycle)
    : _lifetime_id(lifetime_id), _lifecycle(lifecycle) {
}

bool project_settings_opener::can_open() const {
    auto result = this->_lifecycle->lifetime_for_id(this->_lifetime_id) == nullptr;

    return result;
}

void project_settings_opener::open() {
    if (!this->can_open()) {
        return;
    }

    this->_lifecycle->add_lifetime(this->_lifetime_id);
}
