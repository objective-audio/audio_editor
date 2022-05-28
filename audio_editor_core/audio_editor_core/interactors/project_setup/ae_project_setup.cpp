//
//  ae_project_setup.cpp
//

#include "ae_project_setup.h"

#include <audio_editor_core/ae_app_dialog_sub_level_router.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_format_setup.h>
#include <audio_editor_core/ae_project_preparer.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_setup> project_setup::make_shared(project_format_setup *format_setup) {
    auto const &app_level = hierarchy::app_level();
    auto const &preparer = app_level->project_preparer;
    auto const &router = app_level->dialog_level_router;
    return std::make_shared<project_setup>(format_setup, preparer, router);
}

project_setup::project_setup(project_format_setup *format_setup, std::shared_ptr<project_preparer> const &preparer,
                             std::shared_ptr<app_dialog_sub_level_router> const &router)
    : _format_setup(format_setup), _preparer(preparer), _router(router) {
}

void project_setup::select_directory(url const &file_url) {
    if (auto const preparer = this->_preparer.lock()) {
        preparer->prepare(this->_format_setup->format(), file_url);
    }

    this->finalize();
}

void project_setup::finalize() {
    if (auto const router = this->_router.lock()) {
        router->remove_project_format_dialog();
    }

    this->_router.reset();
    this->_preparer.reset();
}