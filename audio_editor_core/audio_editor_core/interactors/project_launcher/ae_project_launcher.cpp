//
//  ae_project_launcher.cpp
//

#include "ae_project_launcher.h"

#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_editor_responder.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_launcher> project_launcher::make_shared(
    identifier const &instance_id, project_format const &project_format,
    project_state_holder_for_project_launcher *state_holder, player *player, timeline_holder const *timeline_holder,
    std::shared_ptr<project_editor_responder> const &responder) {
    return std::make_shared<ae::project_launcher>(instance_id, project_format, state_holder, player, timeline_holder,
                                                  responder);
}

project_launcher::project_launcher(identifier const &instance_id, project_format const &project_format,
                                   project_state_holder_for_project_launcher *state_holder, player *player,
                                   timeline_holder const *timeline_holder,
                                   std::shared_ptr<project_editor_responder> const &responder)
    : _instance_id(instance_id),
      _project_format(project_format),
      _state_holder(state_holder),
      _player(player),
      _timeline_holder(timeline_holder),
      _responder(responder) {
}

void project_launcher::launch() {
    if (this->_state_holder->state() != project_state::launching) {
        assertion_failure_if_not_test();
        return;
    }

    auto const responder = this->_responder.lock();
    if (!responder) {
        assertion_failure_if_not_test();
        return;
    }

    this->_state_holder->set_state(project_state::loading);

    this->_player->set_timeline(this->_timeline_holder->timeline(), this->_project_format.sample_rate,
                                audio::pcm_format::float32);
    this->_player->begin_rendering();

    this->_state_holder->set_state(project_state::editing);
}
