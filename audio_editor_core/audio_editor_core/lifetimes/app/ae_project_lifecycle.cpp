//
//  ae_project_lifecycle.cpp
//

#include "ae_project_lifecycle.h"

#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_lifetime.h>
#include <audio_editor_core/ae_uuid_generator.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_lifecycle> project_lifecycle::make_shared() {
    return std::make_shared<project_lifecycle>(uuid_generator::make_shared());
}

project_lifecycle::project_lifecycle(std::shared_ptr<uuid_generatable> const &uuid_generator)
    : _uuid_generator(uuid_generator) {
}

void project_lifecycle::add_lifetime(url const &project_url, project_id const &project_id,
                                     project_format const &format) {
    auto const lifetime = project_lifetime::make_shared(project_id, format, project_url);

    this->_project_lifetimes->insert_or_replace(project_id, std::make_pair(lifetime, nullptr));

    lifetime->launcher->launch();
}

void project_lifecycle::remove_lifetime(ae::project_id const &project_id) {
    this->_project_lifetimes->erase(project_id);
}

std::shared_ptr<project_lifetime> const &project_lifecycle::lifetime_for_id(ae::project_id const &project_id) const {
    if (this->_project_lifetimes->contains(project_id)) {
        return this->_project_lifetimes->at(project_id).first;
    } else {
        static std::shared_ptr<project_lifetime> const empty = nullptr;
        return empty;
    }
}

std::size_t project_lifecycle::size() const {
    return this->_project_lifetimes->size();
}

observing::syncable project_lifecycle::observe_event(std::function<void(project_lifecycle_event const &)> &&handler) {
    return this->_project_lifetimes->observe([handler = std::move(handler)](
                                                 project_lifecycle::project_lifetimes_t::event const &event) {
        switch (event.type) {
            case observing::map::event_type::any: {
                for (auto const &pair : event.elements) {
                    handler(
                        {.type = project_lifecycle_event_type::inserted, .project_id = pair.second.first->project_id});
                }
            } break;
            case observing::map::event_type::inserted: {
                handler(
                    {.type = project_lifecycle_event_type::inserted, .project_id = event.inserted->first->project_id});
            } break;
            case observing::map::event_type::replaced: {
                handler({.type = project_lifecycle_event_type::erased, .project_id = event.erased->first->project_id});
                handler(
                    {.type = project_lifecycle_event_type::inserted, .project_id = event.inserted->first->project_id});
            } break;
            case observing::map::event_type::erased: {
                handler({.type = project_lifecycle_event_type::erased, .project_id = event.erased->first->project_id});
            } break;
        }
    });
}
