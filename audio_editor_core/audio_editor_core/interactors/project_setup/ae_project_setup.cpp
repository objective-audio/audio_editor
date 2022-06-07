//
//  ae_project_setup.cpp
//

#include "ae_project_setup.h"

#include <audio_editor_core/ae_app_dialog_lifecycle.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_format_setup.h>
#include <audio_editor_core/ae_project_preparer.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_setup> project_setup::make_shared(project_format_setup *format_setup) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &preparer = app_lifetime->project_preparer;
    auto const &lifecycle = app_lifetime->dialog_lifecycle;
    return std::make_shared<project_setup>(format_setup, preparer.get(), lifecycle.get());
}

project_setup::project_setup(project_format_setup *format_setup, project_preparer *preparer,
                             app_dialog_lifecycle *lifecycle)
    : _format_setup(format_setup), _preparer(preparer), _lifecycle(lifecycle) {
}

void project_setup::select_directory(url const &file_url) {
    if (this->_preparer) {
        this->_preparer->prepare(this->_format_setup->format(), file_url);
    }

    this->finalize();
}

void project_setup::finalize() {
    if (this->_lifecycle) {
        this->_lifecycle->remove_project_format_dialog();
    }

    this->_lifecycle = nullptr;
    this->_preparer = nullptr;
}
