//
//  ae_project_setup.cpp
//

#include "ae_project_setup.h"

#include <audio_editor_core/ae_app_modal_lifecycle.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_format_setup.h>
#include <audio_editor_core/ae_window_opener.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_setup> project_setup::make_shared(project_format_setup *format_setup) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &window_opener = app_lifetime->window_opener;
    auto const &lifecycle = app_lifetime->dialog_lifecycle;
    return std::make_shared<project_setup>(format_setup, window_opener.get(), lifecycle.get());
}

project_setup::project_setup(project_format_setup *format_setup, window_opener *opener, app_modal_lifecycle *lifecycle)
    : _format_setup(format_setup), _window_opener(opener), _lifecycle(lifecycle) {
}

void project_setup::select_directory(url const &file_url) {
    if (this->_window_opener) {
        this->_window_opener->open(this->_format_setup->format(), file_url);
    }

    this->finalize();
}

void project_setup::finalize() {
    if (this->_lifecycle) {
        this->_lifecycle->remove_project_setup_dialog();
    }

    this->_lifecycle = nullptr;
    this->_window_opener = nullptr;
}
