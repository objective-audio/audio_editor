//
//  ae_window_lifecycle.cpp
//

#include "ae_window_lifecycle.h"

#include <audio_editor_core/ae_action_id.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <audio_editor_core/ae_window_lifetime.h>
#include <audio_editor_core/ae_window_receiver.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<window_lifecycle> window_lifecycle::make_shared() {
    return std::make_shared<window_lifecycle>();
}

window_lifecycle::window_lifecycle() {
}

void window_lifecycle::add_lifetime(url const &project_dir_url, project_id const &project_id,
                                    project_format const &format) {
    auto const lifetime = window_lifetime::make_shared(project_id, format, project_dir_url);

    this->_window_lifetimes->insert_or_replace(project_id, std::make_pair(lifetime, nullptr));

    lifetime->project_lifecycle->switch_to_project();
}

void window_lifecycle::remove_lifetime(ae::project_id const &project_id) {
    this->_window_lifetimes->erase(project_id);
}

std::shared_ptr<window_lifetime> const &window_lifecycle::lifetime_for_id(ae::project_id const &project_id) const {
    if (this->_window_lifetimes->contains(project_id)) {
        return this->_window_lifetimes->at(project_id).first;
    } else {
        static std::shared_ptr<window_lifetime> const empty = nullptr;
        return empty;
    }
}

std::size_t window_lifecycle::size() const {
    return this->_window_lifetimes->size();
}

observing::syncable window_lifecycle::observe_event(std::function<void(window_lifecycle_event const &)> &&handler) {
    return this->_window_lifetimes->observe([handler = std::move(handler)](
                                                window_lifecycle::window_lifetimes_t::event const &event) {
        switch (event.type) {
            case observing::map::event_type::any: {
                for (auto const &pair : event.elements) {
                    handler(
                        {.type = window_lifecycle_event_type::inserted, .project_id = pair.second.first->project_id});
                }
            } break;
            case observing::map::event_type::inserted: {
                handler(
                    {.type = window_lifecycle_event_type::inserted, .project_id = event.inserted->first->project_id});
            } break;
            case observing::map::event_type::replaced: {
                handler({.type = window_lifecycle_event_type::erased, .project_id = event.erased->first->project_id});
                handler(
                    {.type = window_lifecycle_event_type::inserted, .project_id = event.inserted->first->project_id});
            } break;
            case observing::map::event_type::erased: {
                handler({.type = window_lifecycle_event_type::erased, .project_id = event.erased->first->project_id});
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
    for (auto const &pair : this->_window_lifetimes->elements()) {
        result.emplace_back(pair.second.first->receiver.get());
    }
    return result;
}

std::vector<action_receiver_providable *> window_lifecycle::sub_providers() const {
    std::vector<action_receiver_providable *> result;

    for (auto const &pair : this->_window_lifetimes->elements()) {
        result.emplace_back(pair.second.first->project_lifecycle.get());
    }

    return result;
}
