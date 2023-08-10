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

void module_selector::select(std::vector<module_index> const &indices) {
    auto const inserting = to_vector<selected_module_object>(indices, [](auto const &index) {
        return selected_module_object{index.object_id, {index.range, index.track}};
    });

    this->_selected_pool->insert(inserting);
}

bool module_selector::can_toggle() const {
    return this->_editing_status->can_editing();
}

void module_selector::toggle(module_index const &index) {
    if (this->_module_pool->modules().contains(index)) {
        this->_selected_pool->toggle(selected_module_object{index.object_id, {index.range, index.track}});
    } else {
        assertion_failure_if_not_test();
    }
}
