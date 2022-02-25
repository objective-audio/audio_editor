//
//  ae_root_presenter.cpp
//

#include "ae_root_presenter.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_project_level_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<root_presenter> root_presenter::make_shared(std::string const &project_id) {
    auto const project = app_level::global()->project_pool->project_level_for_id(project_id)->project;
    return std::shared_ptr<root_presenter>(new root_presenter{project});
}

root_presenter::root_presenter(std::shared_ptr<project_for_root_presenter> const &project)
    : _is_editing(observing::value::holder<bool>::make_shared(false)) {
    project->observe_state([this](auto const &state) { this->_is_editing->set_value(state == project_state::editing); })
        .sync()
        ->add_to(this->_pool);
}

observing::syncable root_presenter::observe_is_editing(std::function<void(bool const &)> &&handler) {
    return this->_is_editing->observe(std::move(handler));
}
