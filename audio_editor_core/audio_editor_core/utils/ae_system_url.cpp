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

std::shared_ptr<system_url> system_url::make_shared() {
    return std::shared_ptr<system_url>(new system_url{});
}
