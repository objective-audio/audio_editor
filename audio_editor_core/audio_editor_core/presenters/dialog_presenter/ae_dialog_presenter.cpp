//
//  ae_dialog_presenter.cpp
//

#include "ae_dialog_presenter.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<dialog_presenter> dialog_presenter::make_shared() {
    return std::shared_ptr<dialog_presenter>(new dialog_presenter{});
}

dialog_presenter::dialog_presenter() : _event_notifier(observing::notifier<dialog_event>::make_shared()) {
}

void dialog_presenter::notify_event(dialog_event const &event) {
    this->_event_notifier->notify(event);
}

observing::endable dialog_presenter::observe_event(std::function<void(dialog_event const &)> &&handler) {
    return this->_event_notifier->observe(std::move(handler));
}
