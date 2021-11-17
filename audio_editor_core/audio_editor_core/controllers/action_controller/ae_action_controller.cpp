//
//  ae_action_controller.cpp
//

#include "ae_action_controller.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_controller> action_controller::make_shared() {
    return std::shared_ptr<action_controller>(new action_controller{});
}

action_controller::action_controller()
    : _action_notifier(observing::notifier<action>::make_shared()),
      _export_notifier(observing::notifier<url>::make_shared()) {
}

void action_controller::handle_action(action const action) {
    this->_action_notifier->notify(action);
}

void action_controller::export_to_file(url const &url) {
    this->_export_notifier->notify(url);
}

observing::endable action_controller::observe_action(std::function<void(action const &)> &&handler) {
    return this->_action_notifier->observe(std::move(handler));
}

observing::endable action_controller::observe_export(std::function<void(url const &)> &&handler) {
    return this->_export_notifier->observe(std::move(handler));
}
