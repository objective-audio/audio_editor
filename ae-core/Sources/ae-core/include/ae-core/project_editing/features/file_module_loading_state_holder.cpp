//
//  file_module_loading_state_holder.cpp
//

#include "file_module_loading_state_holder.h"

using namespace yas;
using namespace yas::ae;

file_module_loading_state_holder::file_module_loading_state_holder()
    : _state(observing::value::holder<file_module_loading_state>::make_shared(file_module_loading_state::waiting)) {
}

file_module_loading_state const &file_module_loading_state_holder::state() const {
    return this->_state->value();
}

void file_module_loading_state_holder::set_state(file_module_loading_state const state) {
    this->_state->set_value(state);
}
