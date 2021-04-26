//
//  ae_app.cpp
//

#include "ae_app.h"

#include <audio_editor_core/ae_project.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

app::app() {
}

std::shared_ptr<app> app::make_shared() {
    return std::shared_ptr<app>(new app{});
}

std::shared_ptr<app> app::global() {
    static std::shared_ptr<app> const _global = app::make_shared();
    return _global;
}

std::shared_ptr<project_pool> const &app::project_pool() const {
    return this->_project_pool;
}

std::shared_ptr<system_url> const &app::system_url() const {
    return this->_system_url;
}

std::shared_ptr<file_importer> const &app::file_importer() const {
    return this->_file_importer;
}
