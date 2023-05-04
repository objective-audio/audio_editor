//
//  ae_app_settings_lifetime.cpp
//

#include "ae_app_settings_lifetime.hpp"

#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_hierarchy.h>

#include <audio_editor_core/ae_app_settings_closer.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_settings_lifetime> app_settings_lifetime::make_shared(app_settings_lifetime_id const &lifetime_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    return std::make_shared<app_settings_lifetime>(lifetime_id, app_lifetime.get());
}

app_settings_lifetime::app_settings_lifetime(app_settings_lifetime_id const &lifetime_id,
                                             app_lifetime const *app_lifetime)
    : lifetime_id(lifetime_id),
      closer(std::make_shared<app_settings_closer>(lifetime_id, app_lifetime->app_settings_lifecycle.get())) {
}
