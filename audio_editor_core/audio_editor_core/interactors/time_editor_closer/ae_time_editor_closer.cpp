//
//  ae_time_editor_closer.cpp
//

#include "ae_time_editor_closer.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_timing.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_closer> time_editor_closer::make_shared(window_lifetime_id const &window_lifetime_id,
                                                                    identifier const lifetime_instance_id,
                                                                    time_editor *editor) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    return std::make_shared<time_editor_closer>(lifetime_instance_id, editor, project_lifetime->modal_lifecycle.get(),
                                                project_lifetime->timing.get(), project_lifetime->player.get());
}

time_editor_closer::time_editor_closer(identifier const lifetime_instance_id, time_editor *editor,
                                       project_modal_lifecycle *lifecycle, timing *timing, player *player)
    : _lifetime_instance_id(lifetime_instance_id), _dependencies({editor, lifecycle, timing, player}) {
}

void time_editor_closer::finish() {
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

void time_editor_closer::cancel() {
    if (!this->can_cancel()) {
        assertion_failure_if_not_test();
        return;
    }

    this->_finalize();
}

void time_editor_closer::_finalize() {
    if (!this->_dependencies.has_value()) {
        assertion_failure_if_not_test();
        return;
    }

    auto const &dependencies = this->_dependencies.value();
    auto *lifecycle = dependencies.modal_lifecycle;

    this->_dependencies.reset();

    lifecycle->remove_time_editor();
}

bool time_editor_closer::can_finish() {
    return this->_dependencies.has_value();
}

bool time_editor_closer::can_cancel() {
    return this->_dependencies.has_value();
}
