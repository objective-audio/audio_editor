//
//  ae_escaper.cpp
//

#include "ae_escaper.hpp"

#include <audio_editor_core/ae_pasteboard.h>

#include <audio_editor_core/ae_selected_file_module_pool.hpp>

using namespace yas;
using namespace yas::ae;

escaper::escaper(pasteboard *pasteboard, selected_file_module_pool *selected_pool)
    : _pasteboard(pasteboard), _selected_pool(selected_pool) {
}

bool escaper::can_escape() const {
    return this->_pasteboard->can_clear() || this->_selected_pool->can_clear();
}

void escaper::espace() {
    this->_pasteboard->clear();
    this->_selected_pool->clear();
}
