//
//  marker_selector.cpp
//

#include "marker_selector.hpp"

#include <ae-core/project_editing/features/editing_status.h>
#include <ae-core/project_editing/features/marker_pool.h>
#include <cpp-utils/assertion.h>

#include <ae-core/project_editing/features/deselector.hpp>
#include <ae-core/project_editing/features/selector_enabler.hpp>

using namespace yas;
using namespace yas::ae;

marker_selector::marker_selector(marker_pool const *marker_pool, selected_marker_pool *selected_pool,
                                 editing_status const *editing_status, deselector *deselector,
                                 selector_enabler const *enabler)
    : _marker_pool(marker_pool),
      _selected_pool(selected_pool),
      _editing_status(editing_status),
      _deselector(deselector),
      _enabler(enabler) {
}

bool marker_selector::can_select() const {
    return this->_editing_status->can_editing();
}

void marker_selector::begin_selection() {
    this->_selected_pool->begin_toggling();
}

void marker_selector::select(std::set<marker_index> const &indices) {
    if (this->_enabler->is_enabled(selector_kind::marker)) {
        this->_selected_pool->toggle(indices);
    }
}

void marker_selector::end_selection() {
    this->_selected_pool->end_toggling();
}

bool marker_selector::can_toggle() const {
    return this->_editing_status->can_editing();
}

void marker_selector::toggle(marker_index const &index) {
    if (this->_marker_pool->markers().contains(index)) {
        this->begin_selection();
        this->select({index});
        this->end_selection();
    } else {
        assertion_failure_if_not_test();
    }
}
