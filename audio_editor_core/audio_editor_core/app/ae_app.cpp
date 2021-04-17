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

std::vector<project_ptr> const &app::projects() const {
    return this->_projects->value();
}

observing::syncable app::observe_projects(
    observing::caller<observing::vector::holder<project_ptr>::event>::handler_f &&handler) {
    return this->_projects->observe(std::move(handler));
}

app_ptr app::make_shared() {
    return std::shared_ptr<app>(new app{});
}
