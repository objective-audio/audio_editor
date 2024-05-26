//
//  app_settings_lifecycle_event.h
//

#pragma once

#include <ae-core/global/value_types/app_settings_lifetime_id.hpp>

namespace yas::ae {
enum class app_settings_lifecycle_event_type {
    inserted,
    erased,
};

struct app_settings_lifecycle_event final {
    app_settings_lifecycle_event_type type;
    app_settings_lifetime_id lifetime_id;
};
}  // namespace yas::ae
