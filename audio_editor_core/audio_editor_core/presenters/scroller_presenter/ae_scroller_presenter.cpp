//
//  ae_scroller_presenter.cpp
//

#include "ae_scroller_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_zooming.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<scroller_presenter> scroller_presenter::make_shared(std::string const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    auto const &editor_level = hierarchy::project_editor_level_for_id(project_id);
    return std::shared_ptr<scroller_presenter>(new scroller_presenter{editor_level->file_info, project_level->player,
                                                                      project_level->horizontal_zooming,
                                                                      project_level->vertical_zooming});
}

scroller_presenter::scroller_presenter(file_info const &file_info, std::shared_ptr<player> const &player,
                                       std::shared_ptr<zooming> const &horizontal_zooming,
                                       std::shared_ptr<zooming> const &vertical_zooming)
    : _file_info(file_info),
      _player(player),
      _horizontal_zooming(horizontal_zooming),
      _vertical_zooming(vertical_zooming) {
}

float scroller_presenter::current_position() const {
    if (auto const player = this->_player.lock()) {
        double const sample_rate = this->_file_info.sample_rate;
        return static_cast<double>(player->current_frame()) / sample_rate;
    }
    return 0.0;
}

double scroller_presenter::horizontal_zooming_scale() const {
    if (auto const zooming = this->_horizontal_zooming.lock()) {
        return zooming->scale();
    } else {
        return 1.0;
    }
}
