//
//  ae_markers_presenter.cpp
//

#include "ae_markers_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<markers_presenter> markers_presenter::make_shared(std::string const &project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project);
}

std::shared_ptr<markers_presenter> markers_presenter::make_shared(std::shared_ptr<project> const &project) {
    return std::shared_ptr<markers_presenter>(new markers_presenter{project});
}

markers_presenter::markers_presenter(std::shared_ptr<project> const &project) : _project(project) {
}

std::vector<marker_element> markers_presenter::marker_elements() const {
    if (auto const project = this->_project.lock()) {
        auto const &editor = project->editor();
        auto const &file_info = project->file_info();
        if (editor && file_info) {
            auto const &markers = editor->marker_pool()->markers();
            auto const &sample_rate = file_info.value().sample_rate;

            return to_vector<marker_element>(markers, [&sample_rate](auto const &pair) {
                float const position = float(pair.first) / float(sample_rate);
                return marker_element{position};
            });
        }
    }

    return {};
}

observing::syncable markers_presenter::observe_markers(
    std::function<void(std::vector<marker_element> const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        if (auto const &editor = project->editor()) {
            return editor->marker_pool()->observe_event(
                [this, handler = std::move(handler)](auto const &) { handler(this->marker_elements()); });
        }
    }
    return observing::syncable{};
}
