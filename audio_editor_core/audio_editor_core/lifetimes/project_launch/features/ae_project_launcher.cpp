//
//  ae_project_launcher.cpp
//

#include "ae_project_launcher.h"

#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_launcher> project_launcher::make_shared(project_format const &project_format, player *player,
                                                                timeline_holder const *timeline_holder,
                                                                project_lifecycle *lifecycle) {
    return std::make_shared<ae::project_launcher>(project_format, player, timeline_holder, lifecycle);
}

project_launcher::project_launcher(project_format const &project_format, player *player,
                                   timeline_holder const *timeline_holder, project_lifecycle *lifecycle)
    : _project_format(project_format),
      _player(player),
      _timeline_holder(timeline_holder),
      _project_lifecycle(lifecycle) {
}

void project_launcher::launch() {
    this->_player->set_timeline(this->_timeline_holder->timeline(), this->_project_format.sample_rate,
                                audio::pcm_format::float32);
    this->_player->begin_rendering();

    this->_project_lifecycle->switch_to_project();
}
