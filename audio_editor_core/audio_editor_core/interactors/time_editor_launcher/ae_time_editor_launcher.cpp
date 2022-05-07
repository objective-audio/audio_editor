//
//  ae_time_editor_launcher.cpp
//

#include "ae_time_editor_launcher.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_level.h>
#include <audio_editor_core/ae_time_editor_level_router.h>
#include <audio_editor_core/ae_time_editor_responder.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_launcher> time_editor_launcher::make_shared(project_id const &project_id,
                                                                        timing const *timing,
                                                                        time_editor_level_router *router) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<time_editor_launcher>(
        new time_editor_launcher{project_level->player.get(), timing, router, project_level->responder_stack.get()});
}

time_editor_launcher::time_editor_launcher(player const *player, timing const *timing, time_editor_level_router *router,
                                           responder_stack *responder_stack)
    : _player(player), _timing(timing), _router(router), _responder_stack(responder_stack) {
}

bool time_editor_launcher::can_begin_time_editing() const {
    return this->_router->level() == nullptr;
}

void time_editor_launcher::begin_time_editing(std::optional<std::size_t> const unit_idx) {
    if (!this->can_begin_time_editing()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    auto const components = this->_timing->components(current_frame);

    this->_router->add_level(components.raw_components(), unit_idx);

    auto const &level = this->_router->level();

    this->_responder_stack->push_responder(level->instance_id, level->responder);
}
