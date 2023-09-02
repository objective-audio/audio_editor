//
//  ae_selector_enabler.cpp
//

#include "ae_selector_enabler.hpp"

using namespace yas;
using namespace yas::ae;

selector_enabler::selector_enabler(selected_module_pool const *module_pool, selected_marker_pool const *marker_pool)
    : _module_pool(module_pool), _marker_pool(marker_pool) {
}

bool selector_enabler::is_enabled(selector_kind const kind) const {
    auto const current_kind = this->_current_kind();
    return !current_kind.has_value() || current_kind == kind;
}

bool selector_enabler::is_any_enabled() const {
    return this->_current_kind().has_value();
}

std::optional<selector_kind> selector_enabler::_current_kind() const {
    if (!this->_module_pool->elements().empty()) {
        return selector_kind::module;
    } else if (!this->_marker_pool->elements().empty()) {
        return selector_kind::marker;
    } else {
        return std::nullopt;
    }
}
