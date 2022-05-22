//
//  ui_root_level.h
//

#pragma once

#include <audio_editor_core/ae_marker_location_pool.h>
#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_ui_project_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class display_space;
class keyboard;
class ui_root;
class ui_mesh_data;
class ui_module_waveforms;
class ui_modules;
class ui_edge;
class ui_markers;
class ui_track;
class ui_scroller;
class ui_modal_bg;
class ui_time;
class ui_editing_root;
class waveform_mesh_importer;

struct ui_root_level {
    [[nodiscard]] static std::shared_ptr<ui_root_level> make_shared(std::shared_ptr<ui::standard> const &,
                                                                    ui_project_id const &project_id);

    ui_root_level(std::shared_ptr<ui::standard> const &, ui_project_id const &project_id,
                  std::shared_ptr<waveform_mesh_importer> const &);

    ui_project_id const project_id;

    std::shared_ptr<ui::standard> const standard;
    std::shared_ptr<ui::texture> const texture;
    std::shared_ptr<ui::font_atlas> const font_atlas_14;
    std::shared_ptr<ui_mesh_data> const vertical_line_data;
    std::shared_ptr<ae::display_space> const display_space;
    std::shared_ptr<ae::keyboard> const keyboard;

    std::shared_ptr<module_location_pool> const module_location_pool;
    std::shared_ptr<marker_location_pool> const marker_location_pool;
    std::shared_ptr<waveform_mesh_importer> const waveforms_mesh_importer;
    std::shared_ptr<ui_module_waveforms> const waveforms;
    std::shared_ptr<ui_modules> const modules;
    std::shared_ptr<ui_edge> const edge;
    std::shared_ptr<ui_markers> const markers;
    std::shared_ptr<ui_track> const track;
    std::shared_ptr<ui_scroller> const scroller;

    std::shared_ptr<ui_modal_bg> const modal_bg;
    std::shared_ptr<ui_time> const time;

    std::shared_ptr<ui_editing_root> const editing_root;
    std::shared_ptr<ui_root> const root;
};
}  // namespace yas::ae
