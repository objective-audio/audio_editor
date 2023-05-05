//
//  ae_display_space_types.h
//

#pragma once

#include <ui/yas_ui_types.h>

namespace yas::ae {
enum class display_space_event_source {
    fetched,
    view,
    scale,
};

struct display_space_event final {
    display_space_event_source source;
    ui::region const &region;
    ui::size const &scale;
};
}  // namespace yas::ae
