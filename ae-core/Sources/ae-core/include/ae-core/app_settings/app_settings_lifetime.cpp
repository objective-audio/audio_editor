//
//  app_settings_lifetime.cpp
//

#include "app_settings_lifetime.hpp"

#include <ae-core/app/app_lifetime.h>
#include <ae-core/global/hierarchy.h>

#include <ae-core/app_settings/features/app_settings_closer.hpp>

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
