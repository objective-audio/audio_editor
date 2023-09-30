//
//  ae_ui_base_lifetime.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>

#include <memory>

namespace yas::ui {
class strings;
}

namespace yas::ae {
class project_editing_lifetime;
class ui_resource_lifetime;
class ui_node_hierarchy;
class ui_background;
class ui_grid;
class ui_module_waveforms;
class ui_modules;
class ui_pasting_modules;
class ui_edge;
class ui_markers;
class ui_pasting_markers;
class ui_tracks;
class ui_tracks_bg;
class ui_zooming;
class ui_scroller;
class ui_modal_bg;
class ui_time_numbers;
class ui_time_buttons;
class ui_time_bg;
class ui_time_nudge;
class ui_time_base;
class ui_playing_line;
class ui_event_handling;
class ui_range_selection;
class ui_range_selection_input;

struct ui_base_lifetime final {
    ui_base_lifetime(project_lifetime_id const &);

    project_lifetime_id const project_lifetime_id;

    std::shared_ptr<ui_node_hierarchy> const node_hierarchy;

    std::shared_ptr<ui_background> const background;
    std::shared_ptr<ui_range_selection_input> const range_selection_input;
    std::shared_ptr<ui_grid> const grid;
    std::shared_ptr<ui_module_waveforms> const waveforms;
    std::shared_ptr<ui_modules> const modules;
    std::shared_ptr<ui_range_selection> const range_selection;
    std::shared_ptr<ui_pasting_modules> const pasting_modules;
    std::shared_ptr<ui_edge> const edge;
    std::shared_ptr<ui_markers> const markers;
    std::shared_ptr<ui_pasting_markers> const pasting_markers;
    std::shared_ptr<ui_tracks> const tracks;
    std::shared_ptr<ui_tracks_bg> const tracks_bg;
    std::shared_ptr<ui_zooming> const zooming;
    std::shared_ptr<ui_scroller> const scroller;

    std::shared_ptr<ui_modal_bg> const modal_bg;

    std::shared_ptr<ui_time_base> const time_base;
    std::shared_ptr<ui_time_numbers> const time_numbers;
    std::shared_ptr<ui_time_buttons> const time_buttons;
    std::shared_ptr<ui_time_bg> const time_bg;
    std::shared_ptr<ui_time_nudge> const time_nudge;

    std::shared_ptr<ui_playing_line> const playing_line;
    std::shared_ptr<ui_event_handling> const event_handling;

   private:
    ui_base_lifetime(ui_base_lifetime const &) = delete;
    ui_base_lifetime(ui_base_lifetime &&) = delete;
    ui_base_lifetime &operator=(ui_base_lifetime const &) = delete;
    ui_base_lifetime &operator=(ui_base_lifetime &&) = delete;
};
}  // namespace yas::ae
