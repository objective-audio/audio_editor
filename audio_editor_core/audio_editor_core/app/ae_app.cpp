//
//  ae_app.cpp
//

#include "ae_app.h"

#include <audio_editor_core/ae_project.h>

#include <iostream>

using namespace yas;
using namespace yas::ae;

app::app() {
}

void app::add_project(url const &file_url) {
    this->_projects->push_back(project::make_shared(file_url));
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
