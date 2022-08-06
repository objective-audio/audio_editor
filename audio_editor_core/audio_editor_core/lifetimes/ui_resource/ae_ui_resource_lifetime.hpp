//
//  ae_ui_resource_lifetime.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class display_space;
class keyboard;
class ui_mesh_data;
class ui_base_lifecycle;

struct ui_resource_lifetime final {
    ui_resource_lifetime(std::shared_ptr<ui::standard> const &, window_lifetime_id const &);

    window_lifetime_id const window_lifetime_id;

    std::shared_ptr<ui::standard> const standard;
    std::shared_ptr<ui::texture> const texture;
    std::shared_ptr<ui::font_atlas> const font_atlas_14;
    std::shared_ptr<ui_mesh_data> const vertical_line_data;
    std::shared_ptr<ae::display_space> const display_space;
    std::shared_ptr<ae::keyboard> const keyboard;

    std::shared_ptr<ui_base_lifecycle> const base_lifecycle;
};
}  // namespace yas::ae
