//
//  ae_app.cpp
//

#include "ae_app.h"

#include <audio_editor_core/ae_system_url.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app> app::make_shared(std::shared_ptr<ae::system_url> const &system_url) {
    return std::shared_ptr<app>(new app{system_url});
}

app::app(std::shared_ptr<ae::system_url> const &system_url) : worker(worker::make_shared()), _system_url(system_url) {
}

void app::setup() {
    file_manager::remove_contents_in_directory(this->_system_url->app_directory().path());

    this->worker->start();
}
