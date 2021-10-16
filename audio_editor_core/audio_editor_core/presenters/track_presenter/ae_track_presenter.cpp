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

std::vector<module_element> track_presenter::module_elements() const {
    if (auto const project = this->_project.lock()) {
        auto const &editor = project->editor();
        auto const &file_info = project->file_info();
        if (editor && file_info) {
            auto const &modules = editor->file_track()->modules();
            auto const &sample_rate = file_info.value().sample_rate;

            return to_vector<module_element>(modules, [&sample_rate](auto const &pair) {
                float const x = float(pair.second.range.frame) / float(sample_rate);
                float const width = float(pair.second.range.length) / float(sample_rate);
                return module_element{x, width};
            });
        }
    }

    return {};
}

observing::syncable track_presenter::observe_modules(
    std::function<void(std::vector<module_element> const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        if (auto const &editor = project->editor()) {
            return editor->file_track()->observe_event(
                [this, handler = std::move(handler)](auto const &) { handler(this->module_elements()); });
        }
    }
    return observing::syncable{};
}

double track_presenter::current_time() const {
    if (auto const project = this->_project.lock()) {
        if (auto const &file_info = project->file_info()) {
            auto const sample_rate = file_info.value().sample_rate;
            return double(project->player()->current_frame()) / sample_rate;
        }
    }
    return 0.0;
}
