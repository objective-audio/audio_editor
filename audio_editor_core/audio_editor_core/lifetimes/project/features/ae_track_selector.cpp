//
//  ae_track_selector.cpp
//

#include "ae_track_selector.hpp"

#include <audio_editor_core/ae_editing_status.h>

#include <audio_editor_core/ae_deselector.hpp>
#include <audio_editor_core/ae_selector_enabler.hpp>
#include <audio_editor_core/ae_vertical_scrolling.hpp>

using namespace yas;
using namespace yas::ae;

track_selector::track_selector(selected_track_pool *selected_pool, editing_status const *editing_status,
                               deselector *deselector, selector_enabler const *enabler)
    : _selected_pool(selected_pool), _editing_status(editing_status), _deselector(deselector), _enabler(enabler) {
}

bool track_selector::can_select() const {
    return this->_editing_status->can_editing();
}

void track_selector::begin_selection() {
    this->_selected_pool->begin_toggling();
}

void track_selector::select(std::set<track_index_t> const &indices) {
    if (this->_enabler->is_enabled(selector_kind::track)) {
        this->_selected_pool->toggle(indices);
    }
}

void track_selector::end_selection() {
    this->_selected_pool->end_toggling();
}

bool track_selector::can_toggle() const {
    return this->_editing_status->can_editing();
}

void track_selector::toggle(track_index_t const &index) {
    this->begin_selection();
    this->select({index});
    this->end_selection();
}
