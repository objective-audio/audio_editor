//
//  ae_time_editor_lifetime.cpp
//

#include "ae_time_editor_lifetime.h"

#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_closer.h>
#include <audio_editor_core/ae_time_editor_receiver.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_lifetime> time_editor_lifetime::make_shared(ae::time_editor_lifetime_id const &lifetime_id,
                                                                        number_components const &components) {
    return std::make_shared<time_editor_lifetime>(lifetime_id, components);
}

time_editor_lifetime::time_editor_lifetime(ae::time_editor_lifetime_id const &lifetime_id,
                                           number_components const &components)
    : lifetime_id(lifetime_id),
      editor(time_editor::make_shared(components)),
      closer(time_editor_closer::make_shared(lifetime_id, lifetime_id.instance, this->editor.get())),
      receiver(time_editor_receiver::make_shared(lifetime_id, this->editor.get(), this->closer.get())) {
}
