//
//  ae_settings_opener.cpp
//

#include "ae_settings_opener.hpp"

#include <audio_editor_core/ae_settings_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

settings_opener::settings_opener(window_lifetime_id const lifetime_id, settings_lifecycle *lifecycle)
    : _lifetime_id(lifetime_id), _lifecycle(lifecycle) {
}

bool settings_opener::can_open_settings() const {
    auto result = this->_lifecycle->lifetime_for_id(this->_lifetime_id) == nullptr;

    return result;
}

void settings_opener::open_settings() {
    if (!this->can_open_settings()) {
        return;
    }

    this->_lifecycle->add_lifetime(this->_lifetime_id);
}
