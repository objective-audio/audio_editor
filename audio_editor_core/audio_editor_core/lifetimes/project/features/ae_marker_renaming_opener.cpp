//
//  ae_marker_renaming_opener.cpp
//

#include "ae_marker_renaming_opener.hpp"

#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<marker_renaming_opener> marker_renaming_opener::make_shared(project_modal_lifecycle *lifecycle,
                                                                            editing_status const *status) {
    return std::make_shared<marker_renaming_opener>(lifecycle, status);
}

marker_renaming_opener::marker_renaming_opener(project_modal_lifecycle *lifecycle, editing_status const *status)
    : _lifecycle(lifecycle), _editing_status(status) {
}

bool marker_renaming_opener::can_begin_renaming() const {
    if (!this->_editing_status->can_editing() || this->_lifecycle->current().has_value()) {
        return false;
    }

    return true;
}

void marker_renaming_opener::begin_renaming(frame_index_t const &frame) {
    if (!this->can_begin_renaming()) {
        return;
    }

    this->_lifecycle->add_sheet({.kind = sheet_kind::marker_name, .value = frame});
}
