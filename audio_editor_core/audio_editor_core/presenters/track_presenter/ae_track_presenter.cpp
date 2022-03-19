//
//  ae_track_presenter.cpp
//

#include "ae_track_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_zooming.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<track_presenter> track_presenter::make_shared(std::string const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<track_presenter>(
        new track_presenter{project_level->horizontal_zooming, project_level->vertical_zooming});
}

track_presenter::track_presenter(std::shared_ptr<zooming> const &horizontal_zooming,
                                 std::shared_ptr<zooming> const &vertical_zooming)
    : _horizontal_zooming(horizontal_zooming), _vertical_zooming(vertical_zooming) {
}

double track_presenter::horizontal_zooming_scale() const {
    if (auto const zooming = this->_horizontal_zooming.lock()) {
        return zooming->scale();
    } else {
        return 1.0;
    }
}

double track_presenter::vertical_zooming_scale() const {
    if (auto const zooming = this->_vertical_zooming.lock()) {
        return zooming->scale();
    } else {
        return 1.0;
    }
}

observing::syncable track_presenter::observe_horizontal_zooming_scale(std::function<void(double const &)> &&handler) {
    if (auto const zooming = this->_horizontal_zooming.lock()) {
        return zooming->observe_scale(std::move(handler));
    }
    return observing::syncable{};
}

observing::syncable track_presenter::observe_vertical_zooming_scale(std::function<void(double const &)> &&handler) {
    if (auto const zooming = this->_vertical_zooming.lock()) {
        return zooming->observe_scale(std::move(handler));
    }
    return observing::syncable{};
}
