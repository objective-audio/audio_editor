//
//  ae_system_url.cpp
//

#include "ae_system_url.h"

#include <cpp_utils/yas_system_path_utils.h>

using namespace yas;
using namespace yas::ae;

url system_url::document_directory() const {
    return system_path_utils::directory_url(system_path_utils::dir::document);
}

url system_url::app_directory() const {
    return this->document_directory().appending("audio_editor_app");
}

url system_url::project_directory(std::string const &project_id) const {
    return this->app_directory().appending(project_id);
}

std::shared_ptr<system_url> system_url::make_shared() {
    return std::shared_ptr<system_url>(new system_url{});
}
