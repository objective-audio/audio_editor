//
//  ae_project_level_router.cpp
//

#include "ae_project_level_router.h"

#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_uuid_generator.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_level_router> project_level_router::make_shared(
    std::shared_ptr<file_info_loader> const &file_info_loader) {
    return std::make_shared<project_level_router>(uuid_generator::make_shared(), file_info_loader);
}

project_level_router::project_level_router(std::shared_ptr<uuid_generatable> const &uuid_generator,
                                           std::shared_ptr<file_info_loader> const &file_info_loader)
    : _uuid_generator(uuid_generator), _file_info_loader(file_info_loader) {
}

void project_level_router::add_level(url const &project_url, project_id const &project_id,
                                     project_format const &format) {
    auto const project_level = project_level::make_shared(project_id, format, project_url);

    this->_project_levels->insert_or_replace(project_id, std::make_pair(project_level, nullptr));

    project_level->launcher->launch();
}

void project_level_router::remove_level(ae::project_id const &project_id) {
    this->_project_levels->erase(project_id);
}

std::shared_ptr<project_level> const &project_level_router::level_for_id(ae::project_id const &project_id) const {
    if (this->_project_levels->contains(project_id)) {
        return this->_project_levels->at(project_id).first;
    } else {
        static std::shared_ptr<project_level> const empty = nullptr;
        return empty;
    }
}

std::size_t project_level_router::size() const {
    return this->_project_levels->size();
}

observing::syncable project_level_router::observe_event(
    std::function<void(project_level_router_event const &)> &&handler) {
    return this->_project_levels->observe(
        [handler = std::move(handler)](project_level_router::project_levels_t::event const &event) {
            switch (event.type) {
                case observing::map::event_type::any: {
                    for (auto const &pair : event.elements) {
                        handler({.type = project_level_router_event_type::inserted,
                                 .project_id = pair.second.first->project_id});
                    }
                } break;
                case observing::map::event_type::inserted: {
                    handler({.type = project_level_router_event_type::inserted,
                             .project_id = event.inserted->first->project_id});
                } break;
                case observing::map::event_type::replaced: {
                    handler({.type = project_level_router_event_type::erased,
                             .project_id = event.erased->first->project_id});
                    handler({.type = project_level_router_event_type::inserted,
                             .project_id = event.inserted->first->project_id});
                } break;
                case observing::map::event_type::erased: {
                    handler({.type = project_level_router_event_type::erased,
                             .project_id = event.erased->first->project_id});
                } break;
            }
        });
}
