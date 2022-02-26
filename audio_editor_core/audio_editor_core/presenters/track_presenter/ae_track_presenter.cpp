//
//  ae_track_presenter.cpp
//

#include "ae_track_presenter.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_project_editor_level.h>
#include <audio_editor_core/ae_project_editor_level_pool.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_project_level_pool.h>
#include <audio_editor_core/ae_zooming.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<track_presenter> track_presenter::make_shared(std::string const &project_id) {
    auto const &project_level = app_level::global()->project_pool->project_level_for_id(project_id);
    auto const &editor_level = project_level->editor_level_pool->editor_level();
    return make_shared(editor_level->file_info, editor_level->editor, project_level->horizontal_zooming,
                       project_level->vertical_zooming);
}

std::shared_ptr<track_presenter> track_presenter::make_shared(
    file_info const &file_info, std::shared_ptr<project_editor_for_track_presenter> const &editor,
    std::shared_ptr<zooming_for_track_presenter> const &horizontal_zooming,
    std::shared_ptr<zooming_for_track_presenter> const &vertical_zooming) {
    return std::shared_ptr<track_presenter>(
        new track_presenter{file_info, editor, horizontal_zooming, vertical_zooming});
}

track_presenter::track_presenter(file_info const &file_info,
                                 std::shared_ptr<project_editor_for_track_presenter> const &editor,
                                 std::shared_ptr<zooming_for_track_presenter> const &horizontal_zooming,
                                 std::shared_ptr<zooming_for_track_presenter> const &vertical_zooming)
    : _file_info(file_info),
      _project_editor(editor),
      _horizontal_zooming(horizontal_zooming),
      _vertical_zooming(vertical_zooming) {
}

float track_presenter::current_position() const {
    if (auto const editor = this->_project_editor.lock()) {
        double const sample_rate = this->_file_info.sample_rate;
        return static_cast<double>(editor->current_frame()) / sample_rate;
    }
    return 0.0;
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
