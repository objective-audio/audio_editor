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

    auto canceller = project
                         ->observe_notification([this, project_id = project->identifier()](auto const &notification) {
                             switch (notification) {
                                 case project::notification::should_close: {
                                     this->_projects->erase(project_id);
                                 } break;
                             }
                         })
                         .end();

    this->_projects->insert_or_replace(project->identifier(), std::make_pair(project, std::move(canceller)));
}

std::vector<project_ptr> app::projects() const {
    return to_vector<project_ptr>(this->_projects->elements(), [](auto const &pair) { return pair.second.first; });
}

observing::syncable app::observe_projects(observing::caller<projects_map_t::event>::handler_f &&handler) {
    return this->_projects->observe(std::move(handler));
}

app_ptr app::make_shared() {
    return std::shared_ptr<app>(new app{});
}
