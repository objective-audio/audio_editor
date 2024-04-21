//
//  ae_scroller_presenter.cpp
//

#include "ae_scroller_presenter.h"

#include <ae-core/global/ae_hierarchy.h>
#include <ae-core/project/features/ae_player.h>
#include <ae-core/project/features/ae_zooming_pair.h>
#include <ae-core/ui_base/features/zooming/ae_ui_zooming_constants.h>
#include <cpp-utils/lock.h>

#include <ae-core/project/features/ae_vertical_scrolling.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<scroller_presenter> scroller_presenter::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    return std::make_shared<scroller_presenter>(project_editing_lifetime->project_format, project_lifetime->player,
                                                project_lifetime->vertical_scrolling, project_lifetime->zooming_pair);
}

scroller_presenter::scroller_presenter(project_format const &project_format, std::shared_ptr<player> const &player,
                                       std::shared_ptr<vertical_scrolling> const &vertical_scrolling,
                                       std::shared_ptr<zooming_pair> const &zooming_pair)
    : _project_format(project_format),
      _player(player),
      _vertical_scrolling(vertical_scrolling),
      _zooming_pair(zooming_pair) {
}

ui::point scroller_presenter::modules_position() const {
    if (auto const locked = yas::lock(this->_player, this->_vertical_scrolling, this->_zooming_pair);
        fulfilled(locked)) {
        auto const &[player, vertical_scrolling, zooming_pair] = locked;
        auto const zooming_size = to_zooming_size(zooming_pair->scale());
        double const track_position = vertical_scrolling->track_position();

        double const sample_rate = this->_project_format.sample_rate;
        double const seconds = static_cast<double>(player->current_frame()) / sample_rate;

        float const x = -seconds * zooming_size.width_per_sec;
        float const y = -track_position * zooming_size.height_per_track;
        return ui::point{.x = x, .y = y};
    }

    return ui::point::zero();
}

float scroller_presenter::x() const {
    if (auto const locked = yas::lock(this->_player, this->_zooming_pair); fulfilled(locked)) {
        auto const &[player, zooming_pair] = locked;
        auto const zooming_size = to_zooming_size(zooming_pair->scale());

        double const sample_rate = this->_project_format.sample_rate;
        double const seconds = static_cast<double>(player->current_frame()) / sample_rate;

        return -seconds * zooming_size.width_per_sec;
    } else {
        return 0.0f;
    }
}

float scroller_presenter::y() const {
    if (auto const locked = yas::lock(this->_vertical_scrolling, this->_zooming_pair); fulfilled(locked)) {
        auto const &[scrolling, zooming_pair] = locked;
        double const track = scrolling->track_position();
        auto const zooming_size = to_zooming_size(zooming_pair->scale());

        return -track * zooming_size.height_per_track;
    } else {
        return 0.0f;
    }
}
