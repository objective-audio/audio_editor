//
//  project_setup.cpp
//

#include "project_setup.h"

#include <ae-core/app/features/project_opener.h>
#include <ae-core/app/lifecycles/app_modal_lifecycle.h>
#include <ae-core/global/hierarchy.h>
#include <ae-core/project_format_dialog/features/project_format_setup.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_setup> project_setup::make_shared(project_setup_dialog_lifetime_id const &lifetime_id,
                                                          project_format_setup *format_setup) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_opener = app_lifetime->project_opener;
    auto const &lifecycle = app_lifetime->modal_lifecycle;
    return std::make_shared<project_setup>(lifetime_id, format_setup, project_opener.get(), lifecycle.get());
}

project_setup::project_setup(project_setup_dialog_lifetime_id const &lifetime_id, project_format_setup *format_setup,
                             project_opener *opener, app_modal_lifecycle *lifecycle)
    : _lifetime_id(lifetime_id), _format_setup(format_setup), _project_opener(opener), _lifecycle(lifecycle) {
}

void project_setup::select_directory(std::filesystem::path const &file_path) {
    if (this->_project_opener) {
        this->_project_opener->open(this->_format_setup->format(), file_path);
    }

    this->finalize();
}

void project_setup::finalize() {
    if (this->_lifecycle) {
        this->_lifecycle->remove_project_setup_dialog(this->_lifetime_id);
    }

    this->_lifecycle = nullptr;
    this->_project_opener = nullptr;
}
