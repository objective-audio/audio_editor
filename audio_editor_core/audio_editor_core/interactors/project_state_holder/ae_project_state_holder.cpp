//
//  ae_project_state_holder.cpp
//

#include "ae_project_state_holder.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_state_holder> project_state_holder::make_shared() {
    return std::make_shared<project_state_holder>();
}

project_state_holder::project_state_holder()
    : _state(observing::value::holder<project_state>::make_shared(project_state::launching)) {
}

project_state const &project_state_holder::state() const {
    return this->_state->value();
}

void project_state_holder::set_state(project_state const &state) {
    this->_state->set_value(state);
}

observing::syncable project_state_holder::observe_state(std::function<void(project_state const &)> &&handler) {
    return this->_state->observe(std::move(handler));
}
