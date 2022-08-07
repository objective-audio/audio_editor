//
//  ae_ui_base_lifetime.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

#include <memory>

namespace yas::ae {
class project_lifetime;
class ui_resource_lifetime;
class ui_module_waveforms;
class ui_modules;
class ui_edge;
class ui_markers;
class ui_track;
class ui_scroller;
class ui_modal_bg;
class ui_time;
class ui_editing_root;
class ui_root;

struct ui_base_lifetime final {
    ui_base_lifetime(window_lifetime_id const &);
    ui_base_lifetime(window_lifetime_id const &, project_lifetime *, ui_resource_lifetime *resource_lifetime);

    window_lifetime_id const window_lifetime_id;

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
