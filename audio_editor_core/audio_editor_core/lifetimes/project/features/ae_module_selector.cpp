//
//  ae_module_selector.cpp
//

#include "ae_module_selector.hpp"

#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <cpp_utils/yas_assertion.h>

#include <audio_editor_core/ae_selected_file_module.hpp>
#include <audio_editor_core/ae_selected_file_module_pool.hpp>

using namespace yas;
using namespace yas::ae;

module_selector::module_selector(file_track const *file_track, selected_file_module_pool *selected_pool,
                                 editing_status const *editing_status)
    : _file_track(file_track), _selected_pool(selected_pool), _editing_status(editing_status) {
}

bool module_selector::can_select() const {
    return this->_editing_status->can_editing();
}

void module_selector::select_module_at(file_module_index const &index) {
    if (this->_file_track->modules().contains(index)) {
        this->_selected_pool->clear();
        this->_selected_pool->insert_module(selected_file_module_object{index.object_id, {index.range}});
    } else {
        assertion_failure_if_not_test();
    }
}

bool module_selector::can_toggle() const {
    return this->_editing_status->can_editing();
}

void module_selector::toggle_module_at(file_module_index const &index) {
    if (this->_file_track->modules().contains(index)) {
        this->_selected_pool->toggle_module(selected_file_module_object{index.object_id, {index.range}});
    } else {
        assertion_failure_if_not_test();
    }
}
