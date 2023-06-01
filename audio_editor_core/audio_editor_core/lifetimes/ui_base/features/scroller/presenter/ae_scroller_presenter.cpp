//
//  ae_scroller_presenter.cpp
//

#include "ae_scroller_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_ui_zooming_constants.h>
#include <audio_editor_core/ae_zooming_pair.h>
#include <cpp_utils/yas_lock.h>

#include <audio_editor_core/ae_vertical_scrolling.hpp>

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
        auto const scale = zooming_pair->scale();
        double const track = vertical_scrolling->track();

        double const sample_rate = this->_project_format.sample_rate;
        double const seconds = static_cast<double>(player->current_frame()) / sample_rate;

        float const x = -seconds * ui_zooming_constants::standard_width_per_sec * scale.horizontal;
        float const y = -track * ui_zooming_constants::standard_height_per_track;
        return ui::point{.x = x, .y = y};
    }

    return ui::point::zero();
}

float scroller_presenter::x() const {
    if (auto const locked = yas::lock(this->_player, this->_zooming_pair); fulfilled(locked)) {
        auto const &[player, zooming_pair] = locked;
        auto const scale = zooming_pair->scale();

        double const sample_rate = this->_project_format.sample_rate;
        double const seconds = static_cast<double>(player->current_frame()) / sample_rate;

        return -seconds * ui_zooming_constants::standard_width_per_sec * scale.horizontal;
    } else {
        return 0.0f;
    }
}

float scroller_presenter::y() const {
    if (auto const vertical_scrolling = this->_vertical_scrolling.lock()) {
        double const track = vertical_scrolling->track();

        return -track * ui_zooming_constants::standard_height_per_track;
    } else {
        return 0.0f;
    }
}
