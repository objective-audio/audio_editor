//
//  ae_time_editing_closer.cpp
//

#include "ae_time_editing_closer.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_timing.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editing_closer> time_editing_closer::make_shared(project_sub_lifetime_id const &lifetime_id,
                                                                      identifier const lifetime_instance_id,
                                                                      time_editor *editor) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id.project);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id.project);
    return std::make_shared<time_editing_closer>(lifetime_id, editor, project_editing_lifetime->modal_lifecycle.get(),
                                                 project_editing_lifetime->timing.get(),
                                                 project_lifetime->player.get());
}

time_editing_closer::time_editing_closer(project_sub_lifetime_id const &lifetime_id, time_editor *editor,
                                         project_modal_lifecycle *lifecycle, timing *timing, player *player)
    : _lifetime_id(lifetime_id), _dependencies({editor, lifecycle, timing, player}) {
}

void time_editing_closer::finish() {
    if (!this->can_finish()) {
        assertion_failure_if_not_test();
        return;
    }

    if (!this->_dependencies.has_value()) {
        assertion_failure_if_not_test();
        return;
    }

    auto const &dependencies = this->_dependencies.value();
    auto const editor = dependencies.editor;
    auto const timing = dependencies.timing;
    auto const player = dependencies.player;

    editor->commit_unit_value();

    auto const components = editor->finalized_components();
    auto const frame = timing->frame(timing_components{components});
    player->seek(frame);

    this->_finalize();
}

void time_editing_closer::cancel() {
    if (!this->can_cancel()) {
        assertion_failure_if_not_test();
        return;
    }

    this->_finalize();
}

void time_editing_closer::_finalize() {
    if (!this->_dependencies.has_value()) {
        assertion_failure_if_not_test();
        return;
    }

    auto const &dependencies = this->_dependencies.value();
    auto *lifecycle = dependencies.modal_lifecycle;

    this->_dependencies.reset();

    lifecycle->remove_time_editor(this->_lifetime_id);
}

bool time_editing_closer::can_finish() {
    return this->_dependencies.has_value();
}

bool time_editing_closer::can_cancel() {
    return this->_dependencies.has_value();
}
