//
//  ae_project_pool.cpp
//

#include "ae_project_pool.h"

#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_project_maker.h>
#include <audio_editor_core/ae_uuid_generator.h>

using namespace yas;
using namespace yas::ae;

project_pool::project_pool(std::shared_ptr<project_maker> const &project_maker) : _project_maker(project_maker) {
}

void project_pool::add_project(url const &file_url) {
    this->add_and_return_project(file_url);
}

std::shared_ptr<project> project_pool::add_and_return_project(url const &file_url) {
    auto project_level = this->_project_maker->make(file_url);
    auto const &project = project_level->project;

    auto canceller = project
                         ->observe_event([this, project_id = project->identifier()](auto const &event) {
                             switch (event) {
                                 case project_event::should_close: {
                                     this->_project_levels->erase(project_id);
                                 } break;
                             }
                         })
                         .end();

    this->_project_levels->insert_or_replace(project->identifier(),
                                             std::make_pair(std::move(project_level), std::move(canceller)));

    return project;
}

std::shared_ptr<project_level> const &project_pool::project_level_for_id(std::string const &project_id) const {
    if (this->_project_levels->contains(project_id)) {
        return this->_project_levels->at(project_id).first;
    } else {
        static std::shared_ptr<project_level> const empty = nullptr;
        return empty;
    }
}

std::shared_ptr<project> const &project_pool::project_for_id(std::string const &project_id) const {
    if (this->_project_levels->contains(project_id)) {
        return this->_project_levels->at(project_id).first->project;
    } else {
        static std::shared_ptr<project> const empty_project = nullptr;
        return empty_project;
    }
}

std::size_t project_pool::size() const {
    return this->_project_levels->size();
}

observing::syncable project_pool::observe_event(std::function<void(project_pool_event const &)> &&handler) {
    return this->_project_levels->observe([handler =
                                               std::move(handler)](project_pool::project_levels_t::event const &event) {
        switch (event.type) {
            case observing::map::event_type::any: {
                for (auto const &pair : event.elements) {
                    handler({.type = project_pool_event_type::inserted, .project_id = pair.second.first->identifier});
                }
            } break;
            case observing::map::event_type::inserted: {
                handler({.type = project_pool_event_type::inserted, .project_id = event.inserted->first->identifier});
            } break;
            case observing::map::event_type::replaced: {
                handler({.type = project_pool_event_type::erased, .project_id = event.erased->first->identifier});
                handler({.type = project_pool_event_type::inserted, .project_id = event.inserted->first->identifier});
            } break;
            case observing::map::event_type::erased: {
                handler({.type = project_pool_event_type::erased, .project_id = event.erased->first->identifier});
            } break;
        }
    });
}

std::shared_ptr<project_pool> project_pool::make_shared() {
    return make_shared(project_maker::make_shared());
}

std::shared_ptr<project_pool> project_pool::make_shared(std::shared_ptr<project_maker> const &project_maker) {
    return std::shared_ptr<project_pool>(new project_pool{project_maker});
}
