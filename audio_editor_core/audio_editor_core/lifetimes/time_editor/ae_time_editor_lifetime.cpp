//
//  ae_time_editor_lifetime.cpp
//

#include "ae_time_editor_lifetime.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_closer.h>
#include <audio_editor_core/ae_time_editor_receiver.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_lifetime> time_editor_lifetime::make_shared(project_sub_lifetime_id const &lifetime_id,
                                                                        number_components const &components) {
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id.window);
    return std::make_shared<time_editor_lifetime>(lifetime_id, project_editing_lifetime.get(), components);
}

time_editor_lifetime::time_editor_lifetime(project_sub_lifetime_id const &lifetime_id,
                                           project_editing_lifetime const *project_editing_lifetime,
                                           number_components const &components)
    : lifetime_id(lifetime_id),
      editor(std::make_shared<ae::time_editor>(components)),
      closer(time_editor_closer::make_shared(lifetime_id, lifetime_id.instance, this->editor.get())),
      receiver(std::make_shared<time_editor_receiver>(lifetime_id, project_editing_lifetime->database,
                                                      this->editor.get(), this->closer.get())) {
}
