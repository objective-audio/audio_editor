//
//  ae_time_editor_status.cpp
//

#include "ae_time_editor_status.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_status> time_editor_status::make_shared() {
    return std::shared_ptr<time_editor_status>(new time_editor_status{});
}

time_editor_status::time_editor_status() : _state(observing::value::holder<state>::make_shared(state::editing)) {
}

void time_editor_status::finish() {
    if (this->is_ended()) {
        return;
    }

    this->_state->set_value(state::finished);
}

void time_editor_status::cancel() {
    if (this->is_ended()) {
        return;
    }

    this->_state->set_value(state::canceled);
}

bool time_editor_status::is_finished() const {
    return this->_state->value() == state::finished;
}

bool time_editor_status::is_ended() const {
    auto const state = this->_state->value();
    switch (state) {
        case state::finished:
        case state::canceled:
            return true;
        case state::editing:
            return false;
    }
}

bool time_editor_status::can_finish() const {
    return !this->is_ended();
}

bool time_editor_status::can_cancel() const {
    return !this->is_ended();
}
