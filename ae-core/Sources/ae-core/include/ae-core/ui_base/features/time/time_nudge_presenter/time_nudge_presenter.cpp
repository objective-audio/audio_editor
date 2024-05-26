//
//  time_nudge_presenter.cpp
//

#include "time_nudge_presenter.hpp"

#include <ae-core/global/hierarchy.h>

#include <ae-core/app/features/app_settings.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_nudge_presenter> time_nudge_presenter::make_shared() {
    auto const &app_lifetime = hierarchy::app_lifetime();
    return std::make_shared<time_nudge_presenter>(app_lifetime->app_settings);
}

time_nudge_presenter::time_nudge_presenter(std::shared_ptr<app_settings> const &app_settings)
    : _app_settings(app_settings) {
}

std::optional<std::size_t> time_nudge_presenter::nudging_unit_index() const {
    if (auto const settings = this->_app_settings.lock()) {
        return to_index(settings->timing_unit_kind());
    } else {
        return std::nullopt;
    }
}

observing::syncable time_nudge_presenter::observe_nudging_unit_kind(
    std::function<void(timing_unit_kind const &)> &&handler) {
    if (auto const settings = this->_app_settings.lock()) {
        return settings->observe_timing_unit_kind(std::move(handler));
    } else {
        return observing::syncable{};
    }
}
