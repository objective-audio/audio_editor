//
//  ae_project_level_collector.cpp
//

#include "ae_project_level_collector.h"

#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_uuid_generator.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_level_collector> project_level_collector::make_shared() {
    return make_shared(uuid_generator::make_shared());
}

std::shared_ptr<project_level_collector> project_level_collector::make_shared(
    std::shared_ptr<uuid_generatable> const &uuid_generator) {
    return std::shared_ptr<project_level_collector>(new project_level_collector{uuid_generator});
}

project_level_collector::project_level_collector(std::shared_ptr<uuid_generatable> const &uuid_generator)
    : _uuid_generator(uuid_generator) {
}

void project_level_collector::add_level(url const &file_url) {
    this->add_and_return_level(file_url);
}

std::shared_ptr<project_level> project_level_collector::add_and_return_level(url const &file_url) {
    auto const identifier = this->_uuid_generator->generate();
    auto const project_level = project_level::make_shared(identifier, file_url);
    auto const &project_id = project_level->project_id;

    this->_project_levels->insert_or_replace(project_id, std::make_pair(project_level, nullptr));

    return project_level;
}

void project_level_collector::remove_level(std::string const &project_id) {
    this->_project_levels->erase(project_id);
}

std::shared_ptr<project_level> const &project_level_collector::level_for_id(std::string const &project_id) const {
    if (this->_project_levels->contains(project_id)) {
        return this->_project_levels->at(project_id).first;
    } else {
        static std::shared_ptr<project_level> const empty = nullptr;
        return empty;
    }
}

std::size_t project_level_collector::size() const {
    return this->_project_levels->size();
}

observing::syncable project_level_collector::observe_event(
    std::function<void(project_level_collector_event const &)> &&handler) {
    return this->_project_levels->observe(
        [handler = std::move(handler)](project_level_collector::project_levels_t::event const &event) {
            switch (event.type) {
                case observing::map::event_type::any: {
                    for (auto const &pair : event.elements) {
                        handler({.type = project_level_collector_event_type::inserted,
                                 .project_id = pair.second.first->project_id});
                    }
                } break;
                case observing::map::event_type::inserted: {
                    handler({.type = project_level_collector_event_type::inserted,
                             .project_id = event.inserted->first->project_id});
                } break;
                case observing::map::event_type::replaced: {
                    handler({.type = project_level_collector_event_type::erased,
                             .project_id = event.erased->first->project_id});
                    handler({.type = project_level_collector_event_type::inserted,
                             .project_id = event.inserted->first->project_id});
                } break;
                case observing::map::event_type::erased: {
                    handler({.type = project_level_collector_event_type::erased,
                             .project_id = event.erased->first->project_id});
                } break;
            }
        });
}
