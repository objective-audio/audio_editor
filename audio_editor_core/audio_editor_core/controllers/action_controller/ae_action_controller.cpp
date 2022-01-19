//
//  ae_action_controller.cpp
//

#include "ae_action_controller.h"

#include <audio_editor_core/ae_action_router.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_controller> action_controller::make_shared(std::shared_ptr<action_router> const &router) {
    return std::shared_ptr<action_controller>(new action_controller{router});
}

action_controller::action_controller(std::shared_ptr<action_router> const &router)
    : _router(router),
      _action_notifier(observing::notifier<action>::make_shared()),
      _export_notifier(observing::notifier<url>::make_shared()) {
}

std::shared_ptr<action_router> const &action_controller::router() const {
    return this->_router;
}

void action_controller::handle_action(action const action) {
    if (this->_router->responds_to_action(action)) {
        this->_action_notifier->notify(action);
    }
}

void action_controller::handle_key(ae::key const key) {
    if (auto const action = this->_router->to_action(key)) {
        this->handle_action(action.value());
    }
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
