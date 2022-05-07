//
//  ae_module_renaming_launcher.cpp
//

#include "ae_module_renaming_launcher.h"

#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_sheet_presenter.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<module_renaming_launcher> module_renaming_launcher::make_shared(project_id const &project_id,
                                                                                editing_status const *editing_status) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<module_renaming_launcher>(
        new module_renaming_launcher{project_level->sheet_presenter.get(), editing_status});
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

    this->_sheet_presenter->notify_event({.kind = sheet_kind::module_name, .value = range});
}
