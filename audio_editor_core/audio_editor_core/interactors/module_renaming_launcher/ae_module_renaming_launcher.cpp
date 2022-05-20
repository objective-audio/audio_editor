//
//  ae_module_renaming_launcher.cpp
//

#include "ae_module_renaming_launcher.h"

#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_sheet_presenter.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<module_renaming_launcher> module_renaming_launcher::make_shared(sheet_presenter *sheet_presenter,
                                                                                editing_status const *editing_status) {
    return std::make_shared<module_renaming_launcher>(sheet_presenter, editing_status);
}

module_renaming_launcher::module_renaming_launcher(sheet_presenter *sheet_presenter,
                                                   editing_status const *editing_status)
    : _sheet_presenter(sheet_presenter), _editing_status(editing_status) {
}

bool module_renaming_launcher::can_begin_module_renaming() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return true;
}

void module_renaming_launcher::begin_module_renaming(std::string const &range) {
    if (!this->can_begin_module_renaming()) {
        return;
    }

    this->_sheet_presenter->notify_event({.kind = sheet_kind_escape::module_name, .value = range});
}
