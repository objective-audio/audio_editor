//
//  ae_project_lifecycle.cpp
//

#include "ae_project_lifecycle.h"

#include <audio_editor_core/ae_project_launch_lifetime.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_lifetime.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_project_receiver.h>

using namespace yas;
using namespace yas::ae;

project_lifecycle::project_lifecycle(ae::project_lifetime_id const &project_lifetime_id)
    : project_lifetime_id(project_lifetime_id),
      _current(observing::value::holder<std::optional<project_sub_lifetime>>::make_shared(std::nullopt)) {
}

std::optional<project_sub_lifetime> const &project_lifecycle::current() const {
    return this->_current->value();
}

void project_lifecycle::switch_to_project_launch() {
    auto const lifetime = project_launch_lifetime::make_shared(this->project_lifetime_id);

    this->_current->set_value(lifetime);

    lifetime->launcher->launch();
}

void project_lifecycle::switch_to_project() {
    auto const lifetime = project_lifetime::make_shared(this->project_lifetime_id);

    this->_current->set_value(lifetime);
}

observing::syncable project_lifecycle::observe(
    std::function<void(std::optional<project_sub_lifetime> const &)> &&handler) {
    return this->_current->observe(std::move(handler));
}

#pragma mark - action_receiver_provider

std::optional<action_id> project_lifecycle::receivable_id() const {
    return action_id{this->project_lifetime_id};
}

std::vector<action_receivable *> project_lifecycle::receivers() const {
    using kind = project_sub_lifetime_kind;

    switch (to_kind(this->current())) {
        case kind::project: {
            auto const &lifetime = get<project_lifetime>(this->current());
            return {lifetime->receiver.get()};
        }
        case kind::launch:
        case kind::none:
            return {};
    }
}

std::vector<action_receiver_providable *> project_lifecycle::sub_providers() const {
    switch (to_kind(this->current())) {
        case project_sub_lifetime_kind::project: {
            auto const &lifetime = get<project_lifetime>(this->current());
            return {lifetime->modal_lifecycle.get()};
        }
        case project_sub_lifetime_kind::launch:
        case project_sub_lifetime_kind::none:
            return {};
    }
}
