//
//  ae_module_renaming_opener.cpp
//

#include "ae_module_renaming_opener.h"

#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>

#include <audio_editor_core/ae_deselector.hpp>

using namespace yas;
using namespace yas::ae;

module_renaming_opener::module_renaming_opener(project_modal_lifecycle *lifecycle, editing_status const *editing_status,
                                               deselector *deselector)
    : _lifecycle(lifecycle), _editing_status(editing_status), _deselector(deselector) {
}

bool module_renaming_opener::can_begin_module_renaming() const {
    if (!this->_editing_status->can_editing() || this->_lifecycle->current().has_value()) {
        return false;
    }

    return true;
}

void module_renaming_opener::begin_module_renaming(module_index const &index) {
    if (!this->can_begin_module_renaming()) {
        return;
    }

    this->_deselector->deselect_all();
    this->_lifecycle->add_module_name_sheet(index);
}
