//
//  ae_time_editor_opener.cpp
//

#include "ae_time_editor_opener.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_time_editing_lifetime.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

time_editor_opener::time_editor_opener(player const *player, timing const *timing, project_modal_lifecycle *lifecycle)
    : _player(player), _timing(timing), _lifecycle(lifecycle) {
}

bool time_editor_opener::can_begin_time_editing() const {
    return this->_lifecycle->time_editing_lifetime() == nullptr;
}

void time_editor_opener::begin_time_editing() {
    if (!this->can_begin_time_editing()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    auto const components = this->_timing->components(current_frame);

    this->_lifecycle->add_time_editor(components.raw_components());
}
