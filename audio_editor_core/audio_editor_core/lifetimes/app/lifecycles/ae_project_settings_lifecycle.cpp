//
//  ae_project_settings_lifecycle.cpp
//

#include "ae_project_settings_lifecycle.hpp"

#include <audio_editor_core/ae_project_settings_lifetime.hpp>

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<project_settings_lifetime> const _empty_lifetime = nullptr;

project_settings_lifecycle::project_settings_lifecycle() : _lifetimes(lifetimes_t::make_shared()) {
}

void project_settings_lifecycle::add_lifetime(project_lifetime_id const &lifetime_id) {
    this->_lifetimes->insert_or_replace(lifetime_id, project_settings_lifetime::make_shared(lifetime_id));
}

void project_settings_lifecycle::remove_lifetime(project_lifetime_id const &lifetime_id) {
    this->_lifetimes->erase(lifetime_id);
}

std::shared_ptr<project_settings_lifetime> const &project_settings_lifecycle::lifetime_for_id(
    project_lifetime_id const &lifetime_id) const {
    if (this->_lifetimes->contains(lifetime_id)) {
        return this->_lifetimes->at(lifetime_id);
    } else {
        return _empty_lifetime;
    }
}

observing::syncable project_settings_lifecycle::observe_event(
    std::function<void(project_settings_lifecycle_event const &)> &&handler) {
    return this->_lifetimes->observe(
        [handler = std::move(handler)](project_settings_lifecycle::lifetimes_t::event const &event) {
            switch (event.type) {
                case observing::map::event_type::any: {
                    for (auto const &pair : event.elements) {
                        handler({.type = project_settings_lifecycle_event_type::inserted,
                                 .lifetime_id = pair.second->lifetime_id});
                    }
                } break;
                case observing::map::event_type::inserted: {
                    handler({.type = project_settings_lifecycle_event_type::inserted,
                             .lifetime_id = (*event.inserted)->lifetime_id});
                } break;
                case observing::map::event_type::replaced: {
                    handler({.type = project_settings_lifecycle_event_type::erased,
                             .lifetime_id = (*event.erased)->lifetime_id});
                    handler({.type = project_settings_lifecycle_event_type::inserted,
                             .lifetime_id = (*event.inserted)->lifetime_id});
                } break;
                case observing::map::event_type::erased: {
                    handler({.type = project_settings_lifecycle_event_type::erased,
                             .lifetime_id = (*event.erased)->lifetime_id});
                } break;
            }
        });
}
