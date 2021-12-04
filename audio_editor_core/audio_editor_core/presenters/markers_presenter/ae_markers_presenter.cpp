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
    return make_shared(project->editor());
}

std::shared_ptr<markers_presenter> markers_presenter::make_shared(
    std::shared_ptr<project_editor_for_markers_presenter> const &editor) {
    return std::shared_ptr<markers_presenter>(new markers_presenter{editor});
}

markers_presenter::markers_presenter(std::shared_ptr<project_editor_for_markers_presenter> const &editor)
    : _project_editor(editor) {
}

std::vector<marker_location> markers_presenter::marker_locations() const {
    if (auto const editor = this->_project_editor.lock()) {
        if (editor) {
            auto const &file_info = editor->file_info();
            auto const &markers = editor->markers();
            auto const &sample_rate = file_info.sample_rate;

            return to_vector<marker_location>(markers, [&sample_rate](auto const &pair) {
                double const position = static_cast<double>(pair.first) / static_cast<double>(sample_rate);
                return marker_location{.identifier = pair.second.identifier, .x = static_cast<float>(position)};
            });
        }
    }

    return {};
}

observing::syncable markers_presenter::observe_markers(
    std::function<void(std::vector<marker_location> const &)> &&handler) {
    if (auto const editor = this->_project_editor.lock()) {
        return editor->observe_marker_pool_event(
            [this, handler = std::move(handler)](auto const &) { handler(this->marker_locations()); });
    }
    return observing::syncable{};
}
