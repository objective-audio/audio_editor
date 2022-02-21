//
//  ae_context_menu_presenter.cpp
//

#include "ae_context_menu_presenter.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<context_menu_presenter> context_menu_presenter::make_shared() {
    return std::shared_ptr<context_menu_presenter>(new context_menu_presenter{});
}

context_menu_presenter::context_menu_presenter()
    : _value(observing::value::holder<std::optional<ae::context_menu>>::make_shared(std::nullopt)) {
}

std::optional<context_menu> const &context_menu_presenter::context_menu() const {
    return this->_value->value();
}

void context_menu_presenter::set_context_menu(std::optional<ae::context_menu> const &value) {
    this->_value->set_value(value);
}

observing::syncable context_menu_presenter::observe(
    std::function<void(std::optional<ae::context_menu> const &)> &&handler) {
    return this->_value->observe(std::move(handler));
}
