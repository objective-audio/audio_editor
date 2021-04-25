//
//  ae_project_pool.cpp
//

#include "ae_project_pool.h"

#include <audio_editor_core/ae_project.h>
#include <uuid/uuid.h>

using namespace yas;
using namespace yas::ae;

project_pool::project_pool() {
}

void project_pool::add_project(url const &file_url) {
    this->add_and_return_project(file_url);
}

std::shared_ptr<project> project_pool::add_and_return_project(url const &file_url) {
#warning todo
    auto const project = project::make_shared("uuid", file_url);

    auto canceller = project
                         ->observe_event([this, project_id = project->identifier()](auto const &event) {
                             switch (event) {
                                 case project_event::should_close: {
                                     this->_projects->erase(project_id);
                                 } break;
                             }
                         })
                         .end();

    this->_projects->insert_or_replace(project->identifier(), std::make_pair(project, std::move(canceller)));

    return project;
}

std::shared_ptr<project> project_pool::project_for_id(uintptr_t const project_id) const {
    if (this->_projects->contains(project_id)) {
        return this->_projects->at(project_id).first;
    } else {
        return nullptr;
    }
}

std::size_t project_pool::size() const {
    return this->_projects->size();
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

std::shared_ptr<project_pool> project_pool::make_shared() {
    return std::shared_ptr<project_pool>(new project_pool{});
}
