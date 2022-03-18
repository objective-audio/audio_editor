//
//  ae_ui_editing_root_level.h
//

#pragma once

#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_ui_project_id.h>

#include <memory>

namespace yas::ae {
class ui_module_waveforms;
class ui_modules;
class ui_edge;
class ui_markers;
class ui_track;
class ui_scroller;
class ui_time;
class ui_editing_root;

struct ui_editing_root_level final {
    [[nodiscard]] static std::shared_ptr<ui_editing_root_level> make_shared(ui_project_id const &project_id);

    std::shared_ptr<module_location_pool> const location_pool;
    std::shared_ptr<ui_module_waveforms> const waveforms;
    std::shared_ptr<ui_modules> const modules;
    std::shared_ptr<ui_edge> const edge;
    std::shared_ptr<ui_markers> const markers;
    std::shared_ptr<ui_track> const track;
    std::shared_ptr<ui_scroller> const scroller;

    std::shared_ptr<ui_time> const time;

    std::shared_ptr<ui_editing_root> const editing_root;

   private:
    ui_editing_root_level(ui_project_id const &project_id);
};
}  // namespace yas::ae
