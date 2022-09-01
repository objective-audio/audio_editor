//
//  ae_app.cpp
//

#include "ae_app_launcher.h"

#include <audio_editor_core/ae_system_path.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_launcher> app_launcher::make_shared(std::shared_ptr<workable> const &worker,
                                                        std::shared_ptr<ae::system_path> const &system_path) {
    return std::make_shared<app_launcher>(worker, system_path);
}

app_launcher::app_launcher(std::shared_ptr<workable> const &worker, std::shared_ptr<ae::system_path> const &system_path)
    : _worker(worker), _system_path(system_path) {
}

void app_launcher::launch() {
    auto const system_path = this->_system_path.lock();
    auto const worker = this->_worker.lock();

    if (!system_path || !worker) {
        return;
    }

    file_manager::remove_contents_in_directory(system_path->app_directory());

    worker->start();

    this->_system_path.reset();
    this->_worker.reset();
}
