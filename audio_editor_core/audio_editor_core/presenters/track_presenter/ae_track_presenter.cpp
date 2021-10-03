//
//  ae_track_presenter.cpp
//

#include "ae_track_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>

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

file_track_module_map_t const &track_presenter::modules() const {
    if (auto const project = this->_project.lock()) {
        if (auto const &editor = project->editor()) {
            return editor->file_track()->modules();
        }
    }
    return empty_file_track_modules;
}

std::optional<file_info> track_presenter::file_info() const {
    if (auto const project = this->_project.lock()) {
        return project->file_info();
    }
    return std::nullopt;
}

double track_presenter::current_time() const {
    if (auto const project = this->_project.lock()) {
        auto const &file_info = project->file_info();
        if (file_info.has_value()) {
            auto const sample_rate = file_info.value().sample_rate;
            return double(project->player()->current_frame()) / sample_rate;
        }
    }
    return 0.0;
}

observing::syncable track_presenter::observe(std::function<void(std::nullptr_t const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        if (auto const &editor = project->editor()) {
            return editor->file_track()->observe_event(
                [handler = std::move(handler)](auto const &) { handler(nullptr); });
        }
    }
    return observing::syncable{};
}
