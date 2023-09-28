//
//  ae_escaper.cpp
//

#include "ae_escaper.hpp"

#include <audio_editor_core/ae_pasteboard.h>

#include <audio_editor_core/ae_deselector.hpp>

using namespace yas;
using namespace yas::ae;

escaper::escaper(pasteboard *pasteboard, deselector *deselector) : _pasteboard(pasteboard), _deselector(deselector) {
}

bool escaper::can_escape() const {
    return this->_pasteboard->can_clear() || this->_deselector->can_deselect();
}

void escaper::espace() {
    if (this->_pasteboard->can_clear()) {
        this->_pasteboard->clear();
    } else if (this->_deselector->can_deselect()) {
        this->_deselector->deselect_all();
    }
}
