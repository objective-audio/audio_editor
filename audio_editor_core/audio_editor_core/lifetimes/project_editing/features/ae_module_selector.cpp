//
//  ae_module_selector.cpp
//

#include "ae_module_selector.hpp"

#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_module_pool.h>
#include <cpp_utils/yas_assertion.h>

#include <audio_editor_core/ae_deselector.hpp>
#include <audio_editor_core/ae_selected_module.hpp>
#include <audio_editor_core/ae_track_selector.hpp>

using namespace yas;
using namespace yas::ae;

module_selector::module_selector(module_pool const *module_pool, selected_module_pool *selected_pool,
                                 editing_status const *editing_status)
    : _module_pool(module_pool), _selected_pool(selected_pool), _editing_status(editing_status) {
}

bool module_selector::can_select() const {
    return this->_editing_status->can_editing();
}

void module_selector::begin_selection() {
    this->_selected_pool->begin_toggling();
}

void module_selector::select(std::set<module_index> const &indices) {
    this->_selected_pool->toggle(indices);
}

void module_selector::end_selection() {
    this->_selected_pool->end_toggling();
}

bool module_selector::can_toggle() const {
    return this->_editing_status->can_editing();
}

void module_selector::toggle(module_index const &index) {
    if (this->_module_pool->modules().contains(index)) {
        this->begin_selection();
        this->select({index});
        this->end_selection();
    } else {
        assertion_failure_if_not_test();
    }
}
