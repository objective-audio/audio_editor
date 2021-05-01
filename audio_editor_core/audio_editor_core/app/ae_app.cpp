//
//  ae_app.cpp
//

#include "ae_app.h"

#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>
#include <audio_editor_core/ae_system_url.h>
#include <cpp_utils/yas_fast_each.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

app::app()
    : _project_pool(project_pool::make_shared()),
      _system_url(system_url::make_shared()),
      _file_importer(file_importer::make_shared(this->_worker, static_cast<uint32_t>(worker_priority::file_importing))),
      _file_loader(file_loader::make_shared()) {
    file_manager::remove_contents_in_directory(this->_system_url->app_directory().path());
    this->_worker->start();
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

std::shared_ptr<file_loader> const &app::file_loader() const {
    return this->_file_loader;
}
