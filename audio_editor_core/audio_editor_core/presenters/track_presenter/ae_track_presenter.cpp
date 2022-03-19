//
//  ae_track_presenter.cpp
//

#include "ae_track_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_zooming_pair.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<track_presenter> track_presenter::make_shared(std::string const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<track_presenter>(new track_presenter{project_level->zooming_pair});
}

track_presenter::track_presenter(std::shared_ptr<zooming_pair> const &zooming_pair) : _zooming_pair(zooming_pair) {
}

double track_presenter::horizontal_zooming_scale() const {
    if (auto const zooming_pair = this->_zooming_pair.lock()) {
        return zooming_pair->horizontal->scale();
    } else {
        return 1.0;
    }
}

double track_presenter::vertical_zooming_scale() const {
    if (auto const zooming_pair = this->_zooming_pair.lock()) {
        return zooming_pair->vertical->scale();
    } else {
        return 1.0;
    }
}

observing::syncable track_presenter::observe_horizontal_zooming_scale(std::function<void(double const &)> &&handler) {
    if (auto const zooming_pair = this->_zooming_pair.lock()) {
        return zooming_pair->horizontal->observe_scale(std::move(handler));
    }
    return observing::syncable{};
}

observing::syncable track_presenter::observe_vertical_zooming_scale(std::function<void(double const &)> &&handler) {
    if (auto const zooming_pair = this->_zooming_pair.lock()) {
        return zooming_pair->vertical->observe_scale(std::move(handler));
    }
    return observing::syncable{};
}
