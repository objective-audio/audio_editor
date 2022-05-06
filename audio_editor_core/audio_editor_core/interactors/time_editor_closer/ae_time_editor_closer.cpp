//
//  ae_time_editor_closer.cpp
//

#include "ae_time_editor_closer.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_level_router.h>
#include <audio_editor_core/ae_timing.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_closer> time_editor_closer::make_shared(std::string const &project_id,
                                                                    identifier const level_instance_id,
                                                                    time_editor *editor) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    auto const &editor_level = hierarchy::project_editor_level_for_id(project_id);
    return make_shared(level_instance_id, editor, editor_level->time_editor_level_router.get(),
                       project_level->responder_stack.get(), editor_level->timing.get(), project_level->player.get());
}

std::shared_ptr<time_editor_closer> time_editor_closer::make_shared(identifier const level_instance_id,
                                                                    time_editor *editor,
                                                                    time_editor_level_router *router,
                                                                    responder_stack *responder_stack, timing *timing,
                                                                    player *player) {
    return std::shared_ptr<time_editor_closer>(
        new time_editor_closer{level_instance_id, editor, router, responder_stack, timing, player});
}

time_editor_closer::time_editor_closer(identifier const level_instance_id, time_editor *editor,
                                       time_editor_level_router *router, responder_stack *responder_stack,
                                       timing *timing, player *player)
    : _level_instance_id(level_instance_id), _dependencies({editor, router, responder_stack, timing, player}) {
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
    auto *router = dependencies.router;
    auto *responder_stack = dependencies.responder_stack;

    this->_dependencies.reset();

    responder_stack->pop_responder(this->_level_instance_id);
    router->remove_level();
}

bool time_editor_closer::can_finish() {
    return this->_dependencies.has_value();
}

bool time_editor_closer::can_cancel() {
    return this->_dependencies.has_value();
}
