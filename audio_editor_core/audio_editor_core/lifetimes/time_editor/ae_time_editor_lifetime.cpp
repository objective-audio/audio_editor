//
//  ae_time_editor_lifetime.cpp
//

#include "ae_time_editor_lifetime.h"

#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_closer.h>
#include <audio_editor_core/ae_time_editor_receiver.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_lifetime> time_editor_lifetime::make_shared(ae::project_id const &project_id,
                                                                        number_components const &components,
                                                                        std::optional<std::size_t> const unit_idx) {
    return std::make_shared<time_editor_lifetime>(project_id, components, unit_idx);
}

time_editor_lifetime::time_editor_lifetime(ae::project_id const &project_id, number_components const &components,
                                           std::optional<std::size_t> const unit_idx)
    : project_id(project_id),
      editor(time_editor::make_shared(components, unit_idx)),
      closer(time_editor_closer::make_shared(project_id, this->instance_id, this->editor.get())),
      receiver(time_editor_receiver::make_shared(project_id, this->editor.get(), this->closer.get())) {
}
