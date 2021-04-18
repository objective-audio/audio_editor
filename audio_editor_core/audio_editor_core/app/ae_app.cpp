//
//  ae_app.cpp
//

#include "ae_app.h"

#include <audio_editor_core/ae_project.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

app::app() {
}

void app::add_project(url const &file_url) {
    auto const project = project::make_shared(file_url);

    this->_projects->insert_or_replace(project->identifier(), project);

    auto canceller = project
                         ->observe_notify([this, project_id = project->identifier()](auto const &notification) {
                             switch (notification) {
                                 case project::notification::shouldClose: {
                                     this->_projects->erase(project_id);
                                     this->_cancellers.erase(project_id);
                                 } break;
                             }
                         })
                         .end();

    this->_cancellers.emplace(reinterpret_cast<std::uintptr_t>(project.get()), std::move(canceller));
}

std::map<uintptr_t, project_ptr> const &app::projects() const {
    return this->_projects->elements();
}

observing::syncable app::observe_projects(observing::caller<projects_t::event>::handler_f &&handler) {
    return this->_projects->observe(std::move(handler));
}

app_ptr app::make_shared() {
    return std::shared_ptr<app>(new app{});
}
