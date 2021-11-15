//
//  ae_action_controller.cpp
//

#include "ae_action_controller.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_controller> action_controller::make_shared() {
    return std::shared_ptr<action_controller>(new action_controller{});
}

action_controller::action_controller() : _notifier(observing::notifier<action>::make_shared()) {
}

void action_controller::handle_action(action const action) {
    this->_notifier->notify(action);
}

observing::endable action_controller::observe_action(std::function<void(action const &)> &&handler) {
    return this->_notifier->observe(std::move(handler));
}
