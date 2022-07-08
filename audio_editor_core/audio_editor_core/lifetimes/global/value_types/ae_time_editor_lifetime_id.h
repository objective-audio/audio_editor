//
//  ae_time_editor_lifetime_id.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
struct time_editor_lifetime_id final {
    identifier instance_id;
    window_lifetime_id window;

    bool operator==(time_editor_lifetime_id const &) const;
    bool operator!=(time_editor_lifetime_id const &) const;
    bool operator<(time_editor_lifetime_id const &) const;
};
}  // namespace yas::ae
