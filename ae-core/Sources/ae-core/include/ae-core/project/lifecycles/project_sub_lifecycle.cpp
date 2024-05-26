//
//  project_sub_lifecycle.cpp
//

#include "project_sub_lifecycle.h"

#include <ae-core/project_editing/features/project_editing_receiver.h>
#include <ae-core/project_editing/lifecycles/project_modal_lifecycle.h>
#include <ae-core/project_editing/project_editing_lifetime.h>
#include <ae-core/project_launching/features/project_launcher.h>
#include <ae-core/project_launching/project_launching_lifetime.h>

using namespace yas;
using namespace yas::ae;

project_sub_lifecycle::project_sub_lifecycle(ae::project_lifetime_id const &project_lifetime_id)
    : project_lifetime_id(project_lifetime_id),
      _current(observing::value::holder<std::optional<project_sub_lifetime>>::make_shared(std::nullopt)) {
}

std::optional<project_sub_lifetime> const &project_sub_lifecycle::current() const {
    return this->_current->value();
}

void project_sub_lifecycle::switch_to_project_launch() {
    auto const lifetime = project_launching_lifetime::make_shared(this->project_lifetime_id);

    this->_current->set_value(lifetime);

    lifetime->launcher->launch();
}

void project_sub_lifecycle::switch_to_project() {
    auto const lifetime = project_editing_lifetime::make_shared(this->project_lifetime_id);

    this->_current->set_value(lifetime);
}

observing::syncable project_sub_lifecycle::observe(
    std::function<void(std::optional<project_sub_lifetime> const &)> &&handler) {
    return this->_current->observe(std::move(handler));
}

#pragma mark - action_receiver_provider

std::optional<action_id> project_sub_lifecycle::receivable_id() const {
    return action_id{this->project_lifetime_id};
}

std::vector<action_receivable *> project_sub_lifecycle::receivers() const {
    using kind = project_sub_lifetime_kind;

    switch (to_kind(this->current())) {
        case kind::editing: {
            auto const &lifetime = get<project_editing_lifetime>(this->current());
            return {lifetime->receiver.get()};
        }
        case kind::launching:
        case kind::none:
            return {};
    }
}

std::vector<action_receiver_providable *> project_sub_lifecycle::sub_providers() const {
    switch (to_kind(this->current())) {
        case project_sub_lifetime_kind::editing: {
            auto const &lifetime = get<project_editing_lifetime>(this->current());
            return {lifetime->modal_lifecycle.get()};
        }
        case project_sub_lifetime_kind::launching:
        case project_sub_lifetime_kind::none:
            return {};
    }
}
