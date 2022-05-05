//
//  ae_time_editor_closer.cpp
//

#include "ae_time_editor_closer.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_level_pool.h>
#include <audio_editor_core/ae_timing.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_closer> time_editor_closer::make_shared(std::string const &project_id,
                                                                    identifier const level_instance_id,
                                                                    std::shared_ptr<time_editor> const &editor) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    auto const &editor_level = hierarchy::project_editor_level_for_id(project_id);
    return make_shared(level_instance_id, editor, editor_level->time_editor_level_pool, project_level->responder_stack,
                       editor_level->timing, project_level->player);
}

std::shared_ptr<time_editor_closer> time_editor_closer::make_shared(
    identifier const level_instance_id, std::shared_ptr<time_editor> const &editor,
    std::shared_ptr<time_editor_level_pool> const &level_pool, std::shared_ptr<responder_stack> const &responder_stack,
    std::shared_ptr<timing> const &timing, std::shared_ptr<player> const &player) {
    return std::shared_ptr<time_editor_closer>(
        new time_editor_closer{level_instance_id, editor, level_pool, responder_stack, timing, player});
}

time_editor_closer::time_editor_closer(identifier const level_instance_id, std::shared_ptr<time_editor> const &editor,
                                       std::shared_ptr<time_editor_level_pool> const &level_pool,
                                       std::shared_ptr<responder_stack> const &responder_stack,
                                       std::shared_ptr<timing> const &timing, std::shared_ptr<player> const &player)
    : _level_instance_id(level_instance_id), _dependencies({editor, level_pool, responder_stack, timing, player}) {
}

void time_editor_closer::finish() {
    if (!this->can_finish()) {
        assertion_failure_if_not_test();
        return;
    }

    auto const &dependencies = this->_dependencies.value();
    auto const editor = dependencies.editor.lock();
    auto const timing = dependencies.timing.lock();
    auto const player = dependencies.player.lock();

    if (!editor || !timing || !player) {
        assertion_failure_if_not_test();
        return;
    }

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
    auto const level_pool = dependencies.level_pool.lock();
    auto const responder_stack = dependencies.responder_stack.lock();

    this->_dependencies.reset();

    if (!level_pool || !responder_stack) {
        assertion_failure_if_not_test();
        return;
    }

    responder_stack->pop_responder(this->_level_instance_id);

    level_pool->remove_level();
}

bool time_editor_closer::can_finish() {
    return this->_dependencies.has_value();
}

bool time_editor_closer::can_cancel() {
    return this->_dependencies.has_value();
}
