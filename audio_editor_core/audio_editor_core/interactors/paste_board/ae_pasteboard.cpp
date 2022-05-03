//
//  ae_pasteboard.cpp
//

#include "ae_pasteboard.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<pasteboard> pasteboard::make_shared() {
    return std::shared_ptr<pasteboard>(new pasteboard{});
}

pasteboard::pasteboard()
    : _event_fetcher(observing::fetcher<pasteboard_event>::make_shared([] { return pasteboard_event::fetched; })) {
}

std::optional<pasting_file_module> pasteboard::file_module() const {
    return pasting_file_module::make_value(this->_data);
}

void pasteboard::set_file_module(pasting_file_module const &module) {
    this->_data = module.data();
    this->_event_fetcher->push(pasteboard_event::file_module);
}

std::string const &pasteboard::data() const {
    return this->_data;
}

void pasteboard::revert_data(std::string const &data) {
    this->_data = data;
    this->_event_fetcher->push(pasteboard_event::reverted);
}

observing::syncable pasteboard::observe_event(std::function<void(pasteboard_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
