//
//  ae_deselector.cpp
//

#include "ae_deselector.hpp"

using namespace yas;
using namespace yas::ae;

deselector::deselector(selected_module_pool *selected_module_pool, selected_marker_pool *selected_marker_pool,
                       selected_track_pool *selected_track_pool)
    : _selected_module_pool(selected_module_pool),
      _selected_marker_pool(selected_marker_pool),
      _selected_track_pool(selected_track_pool) {
}

void deselector::deselect_all() {
    this->_selected_module_pool->clear();
    this->_selected_marker_pool->clear();
    this->_selected_track_pool->clear();
}
