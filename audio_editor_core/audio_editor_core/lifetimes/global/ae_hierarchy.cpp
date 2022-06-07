//
//  ae_hierarchy.cpp
//

#include "ae_hierarchy.h"

#include <audio_editor_core/ae_app_dialog_lifecycle.h>
#include <audio_editor_core/ae_project_lifecycle.h>

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<app_lifecycle> const _global_app_lifecycle = app_lifecycle::make_shared();

std::shared_ptr<app_lifecycle> const &hierarchy::app_lifecycle() {
    return _global_app_lifecycle;
}

std::shared_ptr<app_lifetime> const &hierarchy::app_lifetime() {
    return hierarchy::app_lifecycle()->lifetime();
}

std::shared_ptr<project_setup_dialog_lifetime> const &hierarchy::project_setup_dialog_lifetime() {
    return hierarchy::app_lifetime()->dialog_lifecycle->project_setup_dialog_lifetime();
}

std::shared_ptr<project_lifetime> const &hierarchy::project_lifetime_for_id(project_id const &project_id) {
    return hierarchy::app_lifetime()->project_lifecycle->lifetime_for_id(project_id);
}
