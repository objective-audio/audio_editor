//
//  ae_app.cpp
//

#include "ae_app.h"

#include <iostream>

using namespace yas;
using namespace yas::ae;

app::app() {
}

void app::add_project(std::string const &file_path) {
    std::cout << "add_project file_path:" << file_path << std::endl;
}

app_ptr app::make_shared() {
    return std::shared_ptr<app>(new app{});
}
