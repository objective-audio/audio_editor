//
//  ae_marker_renaming_opener.cpp
//

#include "ae_marker_renaming_opener.hpp"

#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>

#include <audio_editor_core/ae_deselector.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<marker_renaming_opener> marker_renaming_opener::make_shared(project_modal_lifecycle *lifecycle,
                                                                            editing_status const *status,
                                                                            deselector *deselector) {
    return std::make_shared<marker_renaming_opener>(lifecycle, status, deselector);
}

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
