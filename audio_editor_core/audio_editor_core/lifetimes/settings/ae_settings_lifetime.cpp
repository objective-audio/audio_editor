//
//  ae_settings_lifetime.cpp
//

#include "ae_settings_lifetime.hpp"

#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_hierarchy.h>

#include <audio_editor_core/ae_settings_closer.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<settings_lifetime> settings_lifetime::make_shared(window_lifetime_id const &lifetime_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(lifetime_id);
    return std::make_shared<settings_lifetime>(lifetime_id, app_lifetime.get(), window_lifetime.get());
}

settings_lifetime::settings_lifetime(window_lifetime_id const &lifetime_id, app_lifetime const *app_lifetime,
                                     window_lifetime const *window_lifetime)
    : lifetime_id(lifetime_id),
      project_format(window_lifetime->project_format),
      closer(std::make_shared<settings_closer>(lifetime_id, app_lifetime->settings_lifecycle.get())) {
}
