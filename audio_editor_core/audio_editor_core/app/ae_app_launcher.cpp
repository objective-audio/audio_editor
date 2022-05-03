//
//  ae_app.cpp
//

#include "ae_app_launcher.h"

#include <audio_editor_core/ae_system_url.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_launcher> app_launcher::make_shared(std::shared_ptr<worker> const &worker,
                                                        std::shared_ptr<ae::system_url> const &system_url) {
    return std::shared_ptr<app_launcher>(new app_launcher{worker, system_url});
}

app_launcher::app_launcher(std::shared_ptr<worker> const &worker, std::shared_ptr<ae::system_url> const &system_url)
    : _worker(worker), _system_url(system_url) {
}

void app_launcher::launch() {
    auto const system_url = this->_system_url.lock();
    auto const worker = this->_worker.lock();

    if (!system_url || !worker) {
        return;
    }

    file_manager::remove_contents_in_directory(system_url->app_directory().path());

    worker->start();
}
