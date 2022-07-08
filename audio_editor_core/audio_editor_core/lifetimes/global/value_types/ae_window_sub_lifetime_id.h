//
//  ae_window_sub_lifetime_id.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
struct window_sub_lifetime_id final {
    identifier instance;
    window_lifetime_id window;

    bool operator==(window_sub_lifetime_id const &) const;
    bool operator!=(window_sub_lifetime_id const &) const;
    bool operator<(window_sub_lifetime_id const &) const;
};

using time_editor_lifetime_id = window_sub_lifetime_id;
using sheet_lifetime_id = window_sub_lifetime_id;
using dialog_lifetime_id = window_sub_lifetime_id;
using context_menu_lifetime_id = window_sub_lifetime_id;
}  // namespace yas::ae
