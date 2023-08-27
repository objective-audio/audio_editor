//
//  ae_marker_selector.cpp
//

#include "ae_marker_selector.hpp"

#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <cpp_utils/yas_assertion.h>

#include <audio_editor_core/ae_deselector.hpp>

using namespace yas;
using namespace yas::ae;

marker_selector::marker_selector(marker_pool const *marker_pool, selected_marker_pool *selected_pool,
                                 editing_status const *editing_status, deselector *deselector)
    : _marker_pool(marker_pool),
      _selected_pool(selected_pool),
      _editing_status(editing_status),
      _deselector(deselector) {
}

bool marker_selector::can_select() const {
    return this->_editing_status->can_editing();
}

void marker_selector::begin_selection() {
    this->_selected_pool->begin_toggling();
}

void marker_selector::select(std::vector<marker_index> const &indices) {
    selected_marker_set inserting;

    for (auto const &index : indices) {
        inserting.emplace(index);
    }

    this->_selected_pool->toggle(std::move(inserting));
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
