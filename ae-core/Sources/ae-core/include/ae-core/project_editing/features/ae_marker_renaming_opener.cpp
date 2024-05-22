//
//  ae_marker_renaming_opener.cpp
//

#include "ae_marker_renaming_opener.hpp"

#include <ae-core/project_editing/features/ae_editing_status.h>
#include <ae-core/project_editing/lifecycles/ae_project_modal_lifecycle.h>

#include <ae-core/project_editing/features/ae_deselector.hpp>

using namespace yas;
using namespace yas::ae;

marker_renaming_opener::marker_renaming_opener(project_modal_lifecycle *lifecycle, editing_status const *status,
                                               deselector *deselector)
    : _lifecycle(lifecycle), _editing_status(status), _deselector(deselector) {
}

bool marker_renaming_opener::can_begin_renaming() const {
    if (!this->_editing_status->can_editing() || this->_lifecycle->current().has_value()) {
        return false;
    }

    return true;
}

void marker_renaming_opener::begin_renaming(marker_index const &index) {
    if (!this->can_begin_renaming()) {
        return;
    }

    this->_deselector->deselect_all();
    this->_lifecycle->add_marker_name_sheet(index);
}
