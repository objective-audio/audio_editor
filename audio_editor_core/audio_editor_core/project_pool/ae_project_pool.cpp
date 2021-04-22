//
//  ae_project_pool.cpp
//

#include "ae_project_pool.h"

#include <audio_editor_core/ae_project.h>

using namespace yas;
using namespace yas::ae;

project_pool::project_pool() {
}

project_ptr project_pool::add_project(url const &file_url) {
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

    return project;
}

project_ptr project_pool::project_for_id(uintptr_t const project_id) const {
    if (this->_projects->contains(project_id)) {
        return this->_projects->at(project_id).first;
    } else {
        return nullptr;
    }
}

std::vector<project_ptr> project_pool::projects() const {
    return to_vector<project_ptr>(this->_projects->elements(), [](auto const &pair) { return pair.second.first; });
}

observing::syncable project_pool::observe_event(std::function<void(project_pool_event const &)> &&handler) {
    return this->_projects->observe([handler = std::move(handler)](project_pool::projects_map_t::event const &event) {
        switch (event.type) {
            case observing::map::event_type::any: {
                for (auto const &pair : event.elements) {
                    handler({.type = project_pool_event_type::inserted, .project_id = pair.second.first->identifier()});
                }
            } break;
            case observing::map::event_type::inserted: {
                handler({.type = project_pool_event_type::inserted, .project_id = event.inserted->first->identifier()});
            } break;
            case observing::map::event_type::replaced: {
                handler({.type = project_pool_event_type::erased, .project_id = event.erased->first->identifier()});
                handler({.type = project_pool_event_type::inserted, .project_id = event.inserted->first->identifier()});
            } break;
            case observing::map::event_type::erased: {
                handler({.type = project_pool_event_type::erased, .project_id = event.erased->first->identifier()});
            } break;
        }
    });
}

project_pool_ptr project_pool::make_shared() {
    return std::shared_ptr<project_pool>(new project_pool{});
}
