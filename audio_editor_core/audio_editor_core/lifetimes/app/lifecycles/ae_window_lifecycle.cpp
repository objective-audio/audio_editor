//
//  ae_window_lifecycle.cpp
//

#include "ae_window_lifecycle.h"

#include <audio_editor_core/ae_action_id.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_id_generator.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <audio_editor_core/ae_uuid_generator.h>
#include <audio_editor_core/ae_window_lifetime.h>
#include <audio_editor_core/ae_window_receiver.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<window_lifetime> const _empty_lifetime = nullptr;

window_lifecycle::window_lifecycle(id_generatable const *id_generator, uuid_generatable const *uuid_generator)
    : _id_generator(id_generator), _uuid_generator(uuid_generator) {
}

void window_lifecycle::add_lifetime(std::filesystem::path const &project_dir_path, project_format const &format) {
    window_lifetime_id const lifetime_id{.instance = this->_id_generator->generate(),
                                         .project = {.raw_value = this->_uuid_generator->generate()}};
    auto const lifetime = window_lifetime::make_shared(lifetime_id, format, project_dir_path);

    this->_lifetimes->insert_or_replace(lifetime_id, lifetime);

    lifetime->project_lifecycle->switch_to_project_launch();
}

void window_lifecycle::remove_lifetime(window_lifetime_id const &lifetime_id) {
    this->_lifetimes->erase(lifetime_id);
}

std::shared_ptr<window_lifetime> const &window_lifecycle::lifetime_for_id(window_lifetime_id const &lifetime_id) const {
    if (this->_lifetimes->contains(lifetime_id)) {
        return this->_lifetimes->at(lifetime_id);
    } else {
        return _empty_lifetime;
    }
}

std::size_t window_lifecycle::size() const {
    return this->_lifetimes->size();
}

observing::syncable window_lifecycle::observe_event(std::function<void(window_lifecycle_event const &)> &&handler) {
    return this->_lifetimes->observe([handler = std::move(handler)](window_lifecycle::lifetimes_t::event const &event) {
        switch (event.type) {
            case observing::map::event_type::any: {
                for (auto const &pair : event.elements) {
                    handler({.type = window_lifecycle_event_type::inserted, .lifetime_id = pair.second->lifetime_id});
                }
            } break;
            case observing::map::event_type::inserted: {
                handler({.type = window_lifecycle_event_type::inserted, .lifetime_id = (*event.inserted)->lifetime_id});
            } break;
            case observing::map::event_type::replaced: {
                handler({.type = window_lifecycle_event_type::erased, .lifetime_id = (*event.erased)->lifetime_id});
                handler({.type = window_lifecycle_event_type::inserted, .lifetime_id = (*event.inserted)->lifetime_id});
            } break;
            case observing::map::event_type::erased: {
                handler({.type = window_lifecycle_event_type::erased, .lifetime_id = (*event.erased)->lifetime_id});
            } break;
        }
    });
}

#pragma mark - action_receiver_provider

std::optional<action_id> window_lifecycle::receivable_id() const {
    return std::nullopt;
}

std::vector<action_receivable *> window_lifecycle::receivers() const {
    std::vector<action_receivable *> result;
    for (auto const &pair : this->_lifetimes->elements()) {
        result.emplace_back(pair.second->receiver.get());
    }
    return result;
}

std::vector<action_receiver_providable *> window_lifecycle::sub_providers() const {
    std::vector<action_receiver_providable *> result;

    for (auto const &pair : this->_lifetimes->elements()) {
        result.emplace_back(pair.second->project_lifecycle.get());
    }

    return result;
}
