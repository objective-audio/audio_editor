//
//  ae_settings_lifecycle.cpp
//

#include "ae_settings_lifecycle.hpp"

#include <audio_editor_core/ae_settings_lifetime.hpp>

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<settings_lifetime> const _empty_lifetime = nullptr;

settings_lifecycle::settings_lifecycle() : _lifetimes(lifetimes_t::make_shared()) {
}

void settings_lifecycle::add_lifetime(window_lifetime_id const &lifetime_id) {
    this->_lifetimes->insert_or_replace(lifetime_id, settings_lifetime::make_shared(lifetime_id));
}

void settings_lifecycle::remove_lifetime(window_lifetime_id const &lifetime_id) {
    this->_lifetimes->erase(lifetime_id);
}

std::shared_ptr<settings_lifetime> const &settings_lifecycle::lifetime_for_id(
    window_lifetime_id const &lifetime_id) const {
    if (this->_lifetimes->contains(lifetime_id)) {
        return this->_lifetimes->at(lifetime_id);
    } else {
        return _empty_lifetime;
    }
}

observing::syncable settings_lifecycle::observe_event(std::function<void(settings_lifecycle_event const &)> &&handler) {
    return this->_lifetimes->observe([handler =
                                          std::move(handler)](settings_lifecycle::lifetimes_t::event const &event) {
        switch (event.type) {
            case observing::map::event_type::any: {
                for (auto const &pair : event.elements) {
                    handler({.type = settings_lifecycle_event_type::inserted, .lifetime_id = pair.second->lifetime_id});
                }
            } break;
            case observing::map::event_type::inserted: {
                handler(
                    {.type = settings_lifecycle_event_type::inserted, .lifetime_id = (*event.inserted)->lifetime_id});
            } break;
            case observing::map::event_type::replaced: {
                handler({.type = settings_lifecycle_event_type::erased, .lifetime_id = (*event.erased)->lifetime_id});
                handler(
                    {.type = settings_lifecycle_event_type::inserted, .lifetime_id = (*event.inserted)->lifetime_id});
            } break;
            case observing::map::event_type::erased: {
                handler({.type = settings_lifecycle_event_type::erased, .lifetime_id = (*event.erased)->lifetime_id});
            } break;
        }
    });
}
