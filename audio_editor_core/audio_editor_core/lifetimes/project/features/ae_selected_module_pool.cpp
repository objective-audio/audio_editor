//
//  ae_selected_module_pool.cpp
//

#include "ae_selected_module_pool.hpp"

#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

selected_module_pool::selected_module_pool() {
    this->_event_fetcher =
        observing::fetcher<event>::make_shared([this] { return event{.type = event_type::fetched}; });
}

selected_module_map const &selected_module_pool::modules() const {
    return this->_modules;
}

bool selected_module_pool::contains(module_index const &index) const {
    return this->_modules.contains(index);
}

void selected_module_pool::toggle_module(selected_module_object const &module) {
    auto const index = module.index();

    if (this->_modules.contains(index)) {
        this->erase_module(index);
    } else {
        this->insert_module(module);
    }
}

void selected_module_pool::insert_module(selected_module_object const &module) {
    auto index = module.index();

    if (this->_modules.contains(index)) {
        assertion_failure_if_not_test();
        return;
    }

    this->_modules.emplace(index, module);

    this->_event_fetcher->push({.type = event_type::inserted, .modules = {{index, module}}});
}

void selected_module_pool::erase_module(module_index const &index) {
    if (!this->_modules.contains(index)) {
        assertion_failure_if_not_test();
        return;
    }

    auto const erasing = this->_modules.at(index);

    this->_modules.erase(index);

    this->_event_fetcher->push({.type = event_type::erased, .modules = {{index, erasing}}});
}

bool selected_module_pool::can_clear() const {
    return !this->_modules.empty();
}

void selected_module_pool::clear() {
    if (!this->can_clear()) {
        return;
    }

    auto erased = std::move(this->_modules);

    this->_modules.clear();

    this->_event_fetcher->push({.type = event_type::erased, .modules = std::move(erased)});
}

observing::syncable selected_module_pool::observe_event(std::function<void(event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
