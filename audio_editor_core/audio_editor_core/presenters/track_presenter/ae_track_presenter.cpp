//
//  ae_track_presenter.cpp
//

#include "ae_track_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<track_presenter> track_presenter::make_shared(std::string const &project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project);
}

std::shared_ptr<track_presenter> track_presenter::make_shared(std::shared_ptr<project> const &project) {
    return std::shared_ptr<track_presenter>(new track_presenter{project});
}

track_presenter::track_presenter(std::shared_ptr<project> const &project) : _project(project) {
}

float track_presenter::current_position() const {
    if (auto const project = this->_project.lock()) {
        if (auto const &file_info = project->file_info()) {
            double const sample_rate = file_info.value().sample_rate;
            return static_cast<double>(project->player()->current_frame()) / sample_rate;
        }
    }
    return 0.0;
}
