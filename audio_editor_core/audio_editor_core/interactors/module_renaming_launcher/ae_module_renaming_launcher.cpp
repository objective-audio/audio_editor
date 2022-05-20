//
//  ae_module_renaming_launcher.cpp
//

#include "ae_module_renaming_launcher.h"

#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_sub_level_router.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<module_renaming_launcher> module_renaming_launcher::make_shared(project_sub_level_router *router,
                                                                                editing_status const *editing_status) {
    return std::make_shared<module_renaming_launcher>(router, editing_status);
}

module_renaming_launcher::module_renaming_launcher(project_sub_level_router *router,
                                                   editing_status const *editing_status)
    : _router(router), _editing_status(editing_status) {
}

bool module_renaming_launcher::can_begin_module_renaming() const {
    if (!this->_editing_status->can_editing() || this->_router->sub_level().has_value()) {
        return false;
    }

    return true;
}

void module_renaming_launcher::begin_module_renaming(std::string const &range) {
    if (!this->can_begin_module_renaming()) {
        return;
    }

    this->_router->add_sheet({.kind = sheet_kind::module_name, .value = range});
}
