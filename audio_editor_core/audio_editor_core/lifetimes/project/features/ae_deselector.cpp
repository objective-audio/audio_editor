//
//  ae_deselector.cpp
//

#include "ae_deselector.hpp"

#include <audio_editor_core/ae_selected_marker_pool.hpp>

using namespace yas;
using namespace yas::ae;

deselector::deselector(selected_module_pool *selected_module_pool, selected_marker_pool *selected_marker_pool)
    : _selected_module_pool(selected_module_pool), _selected_marker_pool(selected_marker_pool) {
}

void deselector::deselect_all() {
    this->_selected_module_pool->clear();
    this->_selected_marker_pool->clear();
}
