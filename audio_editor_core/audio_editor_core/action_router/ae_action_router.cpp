//
//  ae_action_router.cpp
//

#include "ae_action_router.h"

#include <audio_editor_core/ae_action_normal_routing.h>
#include <audio_editor_core/ae_action_time_routing.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_router> action_router::make_shared() {
    return std::shared_ptr<action_router>(new action_router{});
}

action_router::action_router()
    : _kind(observing::value::holder<action_routing_kind>::make_shared(action_routing_kind::normal)) {
    this->_kind
        ->observe([this](action_routing_kind const &kind) {
            switch (kind) {
                case action_routing_kind::normal:
                    this->_routing = action_normal_routing::make_shared();
                    break;
                case action_routing_kind::time:
                    this->_routing = action_time_routing::make_shared();
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);
}

void action_router::set_kind(action_routing_kind const kind) {
    this->_kind->set_value(kind);
}

action_routing_kind const &action_router::kind() const {
    return this->_kind->value();
}

observing::syncable action_router::observe_kind(std::function<void(action_routing_kind const &)> &&handler) {
    return this->_kind->observe(std::move(handler));
}

std::optional<ae::action> action_router::to_action(ae::key const key) const {
    auto action = this->_routing->to_action(key);
    if (action.has_value() && !this->_routing->responds_to_action(action.value())) {
        throw std::runtime_error("action_router - not responds to action.");
    }
    return action;
}

bool action_router::responds_to_action(ae::action const action) const {
    return this->_routing->responds_to_action(action);
}
