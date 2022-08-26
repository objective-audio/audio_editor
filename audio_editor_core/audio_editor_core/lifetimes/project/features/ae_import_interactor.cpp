//
//  ae_import_interactor.cpp
//

#include "ae_import_interactor.h"

#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_file_module_loader.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<import_interactor> import_interactor::make_shared(project_modal_lifecycle *lifecycle,
                                                                  editing_status const *editing_status,
                                                                  file_module_loader *file_module_loader) {
    return std::make_shared<import_interactor>(lifecycle, editing_status, file_module_loader);
}

import_interactor::import_interactor(project_modal_lifecycle *lifecycle, editing_status const *editing_status,
                                     file_module_loader *file_module_loader)
    : _lifecycle(lifecycle), _editing_status(editing_status), _file_module_loader(file_module_loader) {
}

bool import_interactor::can_select_file_for_import() const {
    return this->can_import_from_file() && !this->_lifecycle->current().has_value();
}

void import_interactor::select_file_for_import() {
    if (!this->can_select_file_for_import()) {
        return;
    }

    this->_lifecycle->add_file_import_dialog();
}

bool import_interactor::can_import_from_file() const {
    return this->_editing_status->can_editing();
}

void import_interactor::import_from_file(url const &file_url) {
    if (!this->can_import_from_file()) {
        return;
    }

    this->_file_module_loader->load(file_url);
}
