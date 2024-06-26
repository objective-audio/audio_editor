//
//  project_setup_presenter.cpp
//

#include "project_setup_presenter.h"

#include <ae-core/app/lifecycles/app_modal_lifecycle.h>
#include <ae-core/global/hierarchy.h>
#include <ae-core/project_format_dialog/features/project_setup.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_setup_presenter> project_setup_presenter::make_shared() {
    auto const &lifetime = hierarchy::project_setup_dialog_lifetime();
    return std::make_shared<project_setup_presenter>(lifetime->setup);
}

project_setup_presenter::project_setup_presenter(std::shared_ptr<project_setup> const &setup) : _setup(setup) {
}

void project_setup_presenter::select_directory(std::filesystem::path const &file_path) {
    if (auto const setup = this->_setup.lock()) {
        setup->select_directory(file_path);
    }
}

void project_setup_presenter::did_close_dialog() {
    if (auto const setup = this->_setup.lock()) {
        setup->finalize();
    }
}
