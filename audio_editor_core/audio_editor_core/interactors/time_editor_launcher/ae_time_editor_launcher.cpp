//
//  ae_time_editor_launcher.cpp
//

#include "ae_time_editor_launcher.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_lifetime.h>
#include <audio_editor_core/ae_time_editor_responder.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_launcher> time_editor_launcher::make_shared(player const *player, timing const *timing,
                                                                        project_modal_lifecycle *lifecycle,
                                                                        responder_stack *responder_stack) {
    return std::make_shared<time_editor_launcher>(player, timing, lifecycle, responder_stack);
}

time_editor_launcher::time_editor_launcher(player const *player, timing const *timing,
                                           project_modal_lifecycle *lifecycle, responder_stack *responder_stack)
    : _player(player), _timing(timing), _lifecycle(lifecycle), _responder_stack(responder_stack) {
}

bool time_editor_launcher::can_begin_time_editing() const {
    return this->_lifecycle->time_editor_lifetime() == nullptr;
}

void time_editor_launcher::begin_time_editing(std::optional<std::size_t> const unit_idx) {
    if (!this->can_begin_time_editing()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    auto const components = this->_timing->components(current_frame);

    this->_lifecycle->add_time_editor(components.raw_components(), unit_idx);

    auto const &lifecycle = this->_lifecycle->time_editor_lifetime();

    this->_responder_stack->push_responder(lifecycle->instance_id, lifecycle->responder);
}
