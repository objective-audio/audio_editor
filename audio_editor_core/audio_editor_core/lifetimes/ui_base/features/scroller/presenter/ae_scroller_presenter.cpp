//
//  ae_scroller_presenter.cpp
//

#include "ae_scroller_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_zooming_pair.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<scroller_presenter> scroller_presenter::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(project_lifetime_id);
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    return std::make_shared<scroller_presenter>(project_lifetime->project_format, window_lifetime->player,
                                                window_lifetime->zooming_pair);
}

scroller_presenter::scroller_presenter(project_format const &project_format, std::shared_ptr<player> const &player,
                                       std::shared_ptr<zooming_pair> const &zooming_pair)
    : _project_format(project_format), _player(player), _zooming_pair(zooming_pair) {
}

float scroller_presenter::current_position() const {
    if (auto const player = this->_player.lock()) {
        double const sample_rate = this->_project_format.sample_rate;
        return static_cast<double>(player->current_frame()) / sample_rate;
    }
    return 0.0;
}

double scroller_presenter::horizontal_zooming_scale() const {
    if (auto const zooming_pair = this->_zooming_pair.lock()) {
        return zooming_pair->horizontal->scale();
    } else {
        return 1.0;
    }
}
