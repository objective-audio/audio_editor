//
//  ae_ui_base_lifetime.cpp
//

#include "ae_ui_base_lifetime.hpp"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_lifetime.h>
#include <audio_editor_core/ae_ui_edge.h>
#include <audio_editor_core/ae_ui_event_handling.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_markers.h>
#include <audio_editor_core/ae_ui_modal_bg.h>
#include <audio_editor_core/ae_ui_module_waveforms.h>
#include <audio_editor_core/ae_ui_modules.h>
#include <audio_editor_core/ae_ui_scroller.h>
#include <audio_editor_core/ae_ui_time_buttons.h>
#include <audio_editor_core/ae_ui_track.h>
#include <audio_editor_core/ae_ui_background.hpp>
#include <audio_editor_core/ae_ui_node_hierarchy.hpp>
#include <audio_editor_core/ae_ui_pasting_markers.hpp>
#include <audio_editor_core/ae_ui_pasting_modules.hpp>
#include <audio_editor_core/ae_ui_playing_line.hpp>
#include <audio_editor_core/ae_ui_resource_lifetime.hpp>
#include <audio_editor_core/ae_ui_time_base.hpp>
#include <audio_editor_core/ae_ui_time_bg.hpp>
#include <audio_editor_core/ae_ui_time_nudge.hpp>
#include <audio_editor_core/ae_ui_time_numbers.hpp>

using namespace yas;
using namespace yas::ae;

ui_base_lifetime::ui_base_lifetime(ae::window_lifetime_id const &lifetime_id)
    : window_lifetime_id(lifetime_id),
      node_hierarchy(ui_node_hierarchy::make_shared(lifetime_id)),
      background(ui_background::make_shared(lifetime_id)),
      waveforms(ui_module_waveforms::make_shared(lifetime_id)),
      modules(ui_modules::make_shared(lifetime_id, this->node_hierarchy->modules_node, this->waveforms.get())),
      pasting_modules(ui_pasting_modules::make_shared(lifetime_id, this->node_hierarchy->pasting_modules_node)),
      edge(ui_edge::make_shared(lifetime_id, this->node_hierarchy->edge_node)),
      markers(ui_markers::make_shared(lifetime_id, this->node_hierarchy->markers_node)),
      pasting_markers(ui_pasting_markers::make_shared(lifetime_id, this->node_hierarchy->pasting_markers_node)),
      track(ui_track::make_shared(lifetime_id, this->modules.get(), this->pasting_modules.get())),
      scroller(ui_scroller::make_shared(lifetime_id, this->node_hierarchy->scroller_node.get())),
      modal_bg(ui_modal_bg::make_shared(lifetime_id, this->node_hierarchy->modal_bg_node)),
      time_base(ui_time_base::make_shared(lifetime_id, this->node_hierarchy->time_base_node)),
      time_numbers(ui_time_numbers::make_shared(lifetime_id, this->node_hierarchy->time_strings_node)),
      time_buttons(ui_time_buttons::make_shared(lifetime_id, this->node_hierarchy->time_buttons_node.get(),
                                                this->time_numbers.get())),
      time_bg(ui_time_bg::make_shared(lifetime_id, this->node_hierarchy->time_bg_node,
                                      this->time_numbers->actual_layout_source())),
      time_nudge(
          ui_time_nudge::make_shared(lifetime_id, this->time_numbers.get(), this->node_hierarchy->time_nudge_node)),
      playing_line(ui_playing_line::make_shared(lifetime_id, this->node_hierarchy->playing_line_node)),
      event_handling(ui_event_handling::make_shared(lifetime_id)) {
}
