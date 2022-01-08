//
//  ae_time_presenter.cpp
//

#include "ae_time_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_project_pool.h>
#include <audio_editor_core/ae_time_presenter_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_presenter> time_presenter::make_shared(std::string const project_id) {
    auto const project_editor = app::global()->project_pool()->project_for_id(project_id)->editor();
    return std::shared_ptr<time_presenter>(new time_presenter{project_editor});
}

time_presenter::time_presenter(std::shared_ptr<project_editor_for_time_presenter> const &project_editor)
    : _project_editor(project_editor) {
}

std::string time_presenter::time_text() const {
    if (auto const editor = this->_project_editor.lock()) {
        return time_presenter_utils::time_text(editor->current_frame(), editor->file_info().sample_rate);
    } else {
        return "";
    }
}
