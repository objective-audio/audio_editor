//
//  ae_project_status.cpp
//

#include "ae_project_status.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_status> project_status::make_shared() {
    return std::make_shared<project_status>();
}

project_status::project_status()
    : _state(observing::value::holder<project_state>::make_shared(project_state::launching)) {
}

project_state const &project_status::state() const {
    return this->_state->value();
}

void project_status::set_state(project_state const &state) {
    this->_state->set_value(state);
}

observing::syncable project_status::observe_state(std::function<void(project_state const &)> &&handler) {
    return this->_state->observe(std::move(handler));
}
