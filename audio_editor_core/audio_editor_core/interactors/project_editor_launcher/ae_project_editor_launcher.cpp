//
//  ae_project_editor_launcher.cpp
//

#include "ae_project_editor_launcher.h"

#include <audio_editor_core/ae_file_module_loader.h>
#include <audio_editor_core/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_launcher> project_editor_launcher::make_shared(url const &file_url,
                                                                              file_module_loader *file_module_loader) {
    return std::make_shared<project_editor_launcher>(file_url, file_module_loader);
}

project_editor_launcher::project_editor_launcher(url const &file_url, file_module_loader *file_module_loader)
    : _file_url(file_url), _file_module_loader(file_module_loader) {
}

void project_editor_launcher::launch() {
    this->_file_module_loader->load(this->_file_url);
}
