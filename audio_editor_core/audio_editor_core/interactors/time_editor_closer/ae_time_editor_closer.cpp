//
//  ae_time_editor_closer.cpp
//

#include "ae_time_editor_closer.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_level_pool.h>
#include <audio_editor_core/ae_time_editor_status.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_closer> time_editor_closer::make_shared(std::string const &project_id,
                                                                    identifier const level_instance_id,
                                                                    std::shared_ptr<time_editor_status> const &status,
                                                                    std::shared_ptr<time_editor> const &editor) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    auto const &editor_level = hierarchy::project_editor_level_for_id(project_id);
    return make_shared(level_instance_id, status, editor, editor_level->time_editor_level_pool,
                       project_level->responder_stack, editor_level->timing, project_level->player);
}

std::shared_ptr<time_editor_closer> time_editor_closer::make_shared(
    identifier const level_instance_id, std::shared_ptr<time_editor_status> const &status,
    std::shared_ptr<time_editor> const &editor, std::shared_ptr<time_editor_level_pool> const &level_pool,
    std::shared_ptr<responder_stack> const &responder_stack, std::shared_ptr<timing> const &timing,
    std::shared_ptr<player> const &player) {
    return std::shared_ptr<time_editor_closer>(
        new time_editor_closer{level_instance_id, status, editor, level_pool, responder_stack, timing, player});
}

time_editor_closer::time_editor_closer(identifier const level_instance_id,
                                       std::shared_ptr<time_editor_status> const &status,
                                       std::shared_ptr<time_editor> const &editor,
                                       std::shared_ptr<time_editor_level_pool> const &level_pool,
                                       std::shared_ptr<responder_stack> const &responder_stack,
                                       std::shared_ptr<timing> const &timing, std::shared_ptr<player> const &player)
    : _level_instance_id(level_instance_id),
      _status(status),
      _editor(editor),
      _level_pool(level_pool),
      _responder_stack(responder_stack),
      _timing(timing),
      _player(player) {
}

void time_editor_closer::finish() {
    auto const status = this->_status.lock();
    auto const editor = this->_editor.lock();
    auto const timing = this->_timing.lock();
    auto const player = this->_player.lock();
    if (!status || !editor || !timing || !player) {
        return;
    }

    if (!status->can_finish()) {
        return;
    }

    editor->commit_unit_value();

    status->finish();

    if (auto const components = editor->finalized_components()) {
        auto const frame = timing->frame(timing_components{components.value()});
        player->seek(frame);
    }

    this->_finalize();
}

void time_editor_closer::cancel() {
    auto const status = this->_status.lock();
    auto const editor = this->_editor.lock();
    if (!status || !editor) {
        return;
    }

    if (!status->can_cancel()) {
        return;
    }

    status->cancel();

    this->_finalize();
}

void time_editor_closer::_finalize() {
    auto const level_pool = this->_level_pool.lock();
    auto const responder_stack = this->_responder_stack.lock();
    if (!level_pool || !responder_stack) {
        return;
    }

    responder_stack->pop_responder(this->_level_instance_id);

    level_pool->remove_level();
}
