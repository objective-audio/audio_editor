//
//  ae_system_path.cpp
//

#include "ae_system_path.h"

#include <cpp_utils/yas_system_path_utils.h>

#include <iostream>

using namespace yas;
using namespace yas::ae;

system_path::system_path() {
#ifdef DEBUG
    std::cout << this->app_directory() << std::endl;
#endif
}

std::filesystem::path system_path::document_directory() const {
    return system_path_utils::directory_path(system_path_utils::dir::document);
}

std::filesystem::path system_path::app_directory() const {
    return this->document_directory().append("audio_editor_app");
}

std::filesystem::path system_path::playing_directory() const {
    return this->app_directory().append("playing");
}

void system_path::_log_app_directory() const {
    std::cout << this->app_directory() << std::endl;
}
