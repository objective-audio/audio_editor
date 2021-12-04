//
//  ae_modules_presenter.cpp
//

#include "ae_modules_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<modules_presenter> modules_presenter::make_shared(std::string const &project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return make_shared(project->editor());
}

std::shared_ptr<modules_presenter> modules_presenter::make_shared(
    std::shared_ptr<project_editor_for_modules_presenter> const &editor) {
    return std::shared_ptr<modules_presenter>(new modules_presenter{editor});
}

modules_presenter::modules_presenter(std::shared_ptr<project_editor_for_modules_presenter> const &editor)
    : _project_editor(editor) {
}

std::vector<module_location> modules_presenter::module_locations() const {
    if (auto const editor = this->_project_editor.lock()) {
        if (editor) {
            auto const &file_info = editor->file_info();
            auto const &modules = editor->modules();
            auto const &sample_rate = file_info.sample_rate;

            return to_vector<module_location>(modules, [&sample_rate](auto const &pair) {
                float const x = static_cast<double>(pair.second.range.frame) / static_cast<double>(sample_rate);
                float const width = static_cast<double>(pair.second.range.length) / static_cast<double>(sample_rate);
                return module_location{pair.second.identifier, x, width};
            });
        }
    }

    return {};
}

observing::syncable modules_presenter::observe_modules(
    std::function<void(std::vector<module_location> const &)> &&handler) {
    if (auto const editor = this->_project_editor.lock()) {
        return editor->observe_file_track_event(
            [this, handler = std::move(handler)](auto const &) { handler(this->module_locations()); });
    }
    return observing::syncable{};
}
