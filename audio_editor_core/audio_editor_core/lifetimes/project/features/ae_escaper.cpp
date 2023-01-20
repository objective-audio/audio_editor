//
//  ae_escaper.cpp
//

#include "ae_escaper.hpp"

#include <audio_editor_core/ae_pasteboard.h>

#include <audio_editor_core/ae_selected_marker_pool.hpp>
#include <audio_editor_core/ae_selected_module_pool.hpp>

using namespace yas;
using namespace yas::ae;

escaper::escaper(pasteboard *pasteboard, selected_module_pool *selected_module_pool,
                 selected_marker_pool *selected_marker_pool)
    : _pasteboard(pasteboard),
      _selected_module_pool(selected_module_pool),
      _selected_marker_pool(selected_marker_pool) {
}

bool escaper::can_escape() const {
    return this->_pasteboard->can_clear() || this->_selected_module_pool->can_clear() ||
           this->_selected_marker_pool->can_clear();
}

void escaper::espace() {
    this->_pasteboard->clear();
    this->_selected_module_pool->clear();
    this->_selected_marker_pool->clear();
}
