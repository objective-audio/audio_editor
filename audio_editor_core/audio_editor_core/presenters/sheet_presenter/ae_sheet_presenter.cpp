//
//  ae_sheet_presenter.cpp
//

#include "ae_sheet_presenter.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<sheet_presenter> sheet_presenter::make_shared() {
    return std::make_shared<sheet_presenter>();
}

sheet_presenter::sheet_presenter() : _event_notifier(observing::notifier<sheet_event>::make_shared()) {
}

void sheet_presenter::notify_event(sheet_event const &event) {
    this->_event_notifier->notify(event);
}

observing::endable sheet_presenter::observe_event(std::function<void(sheet_event const &)> &&handler) {
    return this->_event_notifier->observe(std::move(handler));
}
