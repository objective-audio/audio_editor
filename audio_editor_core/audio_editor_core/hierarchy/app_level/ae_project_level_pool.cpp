//
//  ae_project_level_pool.cpp
//

#include "ae_project_level_pool.h"

#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_uuid_generator.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_level_pool> project_level_pool::make_shared() {
    return make_shared(uuid_generator::make_shared());
}

std::shared_ptr<project_level_pool> project_level_pool::make_shared(
    std::shared_ptr<uuid_generatable> const &uuid_generator) {
    return std::shared_ptr<project_level_pool>(new project_level_pool{uuid_generator});
}

project_level_pool::project_level_pool(std::shared_ptr<uuid_generatable> const &uuid_generator)
    : _uuid_generator(uuid_generator) {
}

void project_level_pool::add_level(url const &file_url) {
    this->add_and_return_level(file_url);
}

std::shared_ptr<project_level> project_level_pool::add_and_return_level(url const &file_url) {
    auto const identifier = this->_uuid_generator->generate();
    auto const project_level = project_level::make_shared(identifier, file_url);
    auto const &project_id = project_level->project_id;
    auto const &closer = project_level->closer;

    auto canceller = closer
                         ->observe_event([this, project_id](auto const &event) {
                             switch (event) {
                                 case project_event::should_close: {
                                     this->_project_levels->erase(project_id);
                                 } break;
                             }
                         })
                         .end();

    this->_project_levels->insert_or_replace(project_id, std::make_pair(project_level, std::move(canceller)));

    return project_level;
}

std::shared_ptr<project_level> const &project_level_pool::level_for_id(std::string const &project_id) const {
    if (this->_project_levels->contains(project_id)) {
        return this->_project_levels->at(project_id).first;
    } else {
        static std::shared_ptr<project_level> const empty = nullptr;
        return empty;
    }
}

std::size_t project_level_pool::size() const {
    return this->_project_levels->size();
}

observing::syncable project_level_pool::observe_event(std::function<void(project_level_pool_event const &)> &&handler) {
    return this->_project_levels->observe([handler = std::move(handler)](
                                              project_level_pool::project_levels_t::event const &event) {
        switch (event.type) {
            case observing::map::event_type::any: {
                for (auto const &pair : event.elements) {
                    handler(
                        {.type = project_level_pool_event_type::inserted, .project_id = pair.second.first->project_id});
                }
            } break;
            case observing::map::event_type::inserted: {
                handler(
                    {.type = project_level_pool_event_type::inserted, .project_id = event.inserted->first->project_id});
            } break;
            case observing::map::event_type::replaced: {
                handler({.type = project_level_pool_event_type::erased, .project_id = event.erased->first->project_id});
                handler(
                    {.type = project_level_pool_event_type::inserted, .project_id = event.inserted->first->project_id});
            } break;
            case observing::map::event_type::erased: {
                handler({.type = project_level_pool_event_type::erased, .project_id = event.erased->first->project_id});
            } break;
        }
    });
}
