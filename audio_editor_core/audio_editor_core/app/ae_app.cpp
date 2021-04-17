//
//  ae_app.cpp
//

#include "ae_app.h"

#include <iostream>

using namespace yas;
using namespace yas::ae;

app::app() {
}

void app::add_project(url const &file_url) {
    std::cout << "add_project file_url:" << file_url << std::endl;
}

app_ptr app::make_shared() {
    return std::shared_ptr<app>(new app{});
}
