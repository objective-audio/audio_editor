//
//  ae_window_lifecycle_event.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
enum class window_lifecycle_event_type {
    inserted,
    erased,
};

struct window_lifecycle_event final {
    window_lifecycle_event_type type;
    window_lifetime_id lifetime_id;
};
}  // namespace yas::ae
