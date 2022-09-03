//
//  ae_ui_base_lifetime.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

#include <memory>

namespace yas::ui {
class strings;
}

namespace yas::ae {
class project_lifetime;
class ui_resource_lifetime;
class ui_node_hierarchy;
class ui_background;
class ui_module_waveforms;
class ui_modules;
class ui_edge;
class ui_markers;
class ui_track;
class ui_scroller;
class ui_modal_bg;
class ui_time_numbers;
class ui_time_buttons;
class ui_time_bg;
class ui_time_nudge;
class ui_time_base;
class ui_playing_line;
class ui_event_handling;

struct ui_base_lifetime final {
    ui_base_lifetime(window_lifetime_id const &);

    window_lifetime_id const window_lifetime_id;

    std::shared_ptr<ui_node_hierarchy> const node_hierarchy;

    std::shared_ptr<ui_background> const background;
    std::shared_ptr<ui_module_waveforms> const waveforms;
    std::shared_ptr<ui_modules> const modules;
    std::shared_ptr<ui_edge> const edge;
    std::shared_ptr<ui_markers> const markers;
    std::shared_ptr<ui_track> const track;
    std::shared_ptr<ui_scroller> const scroller;

    std::shared_ptr<ui_modal_bg> const modal_bg;

    std::shared_ptr<ui_time_base> const time_base;
    std::shared_ptr<ui_time_numbers> const time_numbers;
    std::shared_ptr<ui_time_buttons> const time_buttons;
    std::shared_ptr<ui_time_bg> const time_bg;
    std::shared_ptr<ui_time_nudge> const time_nudge;

    std::shared_ptr<ui_playing_line> const playing_line;
    std::shared_ptr<ui_event_handling> const event_handling;
};
}  // namespace yas::ae