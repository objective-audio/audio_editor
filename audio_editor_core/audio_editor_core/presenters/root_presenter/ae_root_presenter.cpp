//
//  ae_root_presenter.cpp
//

#include "ae_root_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<root_presenter> root_presenter::make_shared(std::string const &project_id) {
    auto const project = app::global()->project_pool()->project_for_id(project_id);
    return std::shared_ptr<root_presenter>(new root_presenter{project});
}

root_presenter::root_presenter(std::shared_ptr<project> const &project)
    : _project(project), _is_editing(observing::value::holder<bool>::make_shared(false)) {
    project->observe_state([this](auto const &state) { this->_is_editing->set_value(state == project_state::editing); })
        .sync()
        ->add_to(this->_pool);
}

bool root_presenter::responds_to_action(action const action) {
    auto const project = this->_project.lock();
    if (!project) {
        return false;
    }

    switch (action) {
        case action::toggle_play:
            return true;
        case action::nudge_previous:
            return project->can_nudge();
        case action::nudge_next:
            return project->can_nudge();

        case action::jump_previous:
            return project->can_jump_to_previous_edge();
        case action::jump_next:
            return project->can_jump_to_next_edge();

        case action::drop_head_and_offset:
            return project->can_split();
        case action::split:
            return project->can_split();
        case action::drop_tail_and_offset:
            return project->can_split();

        case action::erase_and_offset:
            return project->can_erase();

        case action::insert_marker:
            return project->can_insert_marker();

        case action::return_to_zero:
            return project->can_return_to_zero();
    }
}

observing::syncable root_presenter::observe_is_editing(std::function<void(bool const &)> &&handler) {
    return this->_is_editing->observe(std::move(handler));
}
