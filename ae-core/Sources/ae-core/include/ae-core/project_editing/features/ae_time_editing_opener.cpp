//
//  ae_time_editing_opener.cpp
//

#include "ae_time_editing_opener.h"

#include <ae-core/global/ae_hierarchy.h>
#include <ae-core/project/features/ae_player.h>
#include <ae-core/project_editing/features/ae_timing.h>
#include <ae-core/project_editing/lifecycles/ae_project_modal_lifecycle.h>
#include <ae-core/time_editor/ae_time_editing_lifetime.h>
#include <ae-core/time_editor/features/ae_time_editor.h>

using namespace yas;
using namespace yas::ae;

time_editing_opener::time_editing_opener(player const *player, timing const *timing, project_modal_lifecycle *lifecycle)
    : _player(player), _timing(timing), _lifecycle(lifecycle) {
}

bool time_editing_opener::can_begin_time_editing() const {
    return this->_lifecycle->time_editing_lifetime() == nullptr;
}

void time_editing_opener::begin_time_editing() {
    if (!this->can_begin_time_editing()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    auto const components = this->_timing->components(current_frame);

    this->_lifecycle->add_time_editor(components.raw_components());
}
