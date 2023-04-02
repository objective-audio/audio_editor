//
//  ae_time_nudge_presenter.cpp
//

#include "ae_time_nudge_presenter.hpp"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_nudging.h>
#include <audio_editor_core/ae_project_lifetime.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_nudge_presenter> time_nudge_presenter::make_shared(window_lifetime_id const &window_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    return std::make_shared<time_nudge_presenter>(project_lifetime->nudging);
}

time_nudge_presenter::time_nudge_presenter(std::shared_ptr<nudging> const &nudging) : _nudging(nudging) {
}

std::optional<std::size_t> time_nudge_presenter::nudging_unit_index() const {
    if (auto const settings = this->_nudging.lock()) {
        return settings->unit_index();
    } else {
        return std::nullopt;
    }
}

observing::syncable time_nudge_presenter::observe_nudging_unit_kind(
    std::function<void(timing_unit_kind const &)> &&handler) {
    if (auto const settings = this->_nudging.lock()) {
        return settings->observe_kind(std::move(handler));
    } else {
        return observing::syncable{};
    }
}
