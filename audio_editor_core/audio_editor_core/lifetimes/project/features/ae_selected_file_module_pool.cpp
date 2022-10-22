//
//  ae_selected_file_module_pool.cpp
//

#include "ae_selected_file_module_pool.hpp"

#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<selected_file_module_pool> selected_file_module_pool::make_shared() {
    return std::make_shared<selected_file_module_pool>();
}

selected_file_module_pool::selected_file_module_pool() {
    this->_event_fetcher = observing::fetcher<event>::make_shared(
        [this] { return event{.type = event_type::any, .modules = this->_modules}; });
}

selected_file_module_pool::module_map const &selected_file_module_pool::modules() const {
    return this->_modules;
}

bool selected_file_module_pool::contains(file_module_index const &index) const {
    return this->_modules.contains(index);
}

void selected_file_module_pool::toggle_module(selected_file_module_object const &module) {
    auto const index = module.index();

    if (this->_modules.contains(index)) {
        this->erase_module(index);
    } else {
        this->insert_module(module);
    }
}

void selected_file_module_pool::insert_module(selected_file_module_object const &module) {
    auto index = module.index();

    if (this->_modules.contains(index)) {
        assertion_failure_if_not_test();
        return;
    }

    this->_modules.emplace(std::move(index), module);

    this->_event_fetcher->push({.type = event_type::inserted, .module = module, .modules = this->_modules});
}

void selected_file_module_pool::erase_module(file_module_index const &index) {
    if (!this->_modules.contains(index)) {
        assertion_failure_if_not_test();
        return;
    }

    auto const erasing = this->_modules.at(index);

    this->_modules.erase(index);

    this->_event_fetcher->push({.type = event_type::erased, .module = erasing, .modules = this->_modules});
}

void selected_file_module_pool::clear() {
    if (this->_modules.empty()) {
        return;
    }

    this->_modules.clear();

    this->_event_fetcher->push({.type = event_type::any, .modules = this->_modules});
}

observing::syncable selected_file_module_pool::observe_event(std::function<void(event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
