//
//  ae_ui_resource_lifetime.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class display_space;
class modifiers_holder;
class keyboard;
class ui_mesh_data;
class ui_base_lifecycle;
class ui_atlas;
class ui_square_mesh_data;

struct ui_resource_lifetime final {
    ui_resource_lifetime(std::shared_ptr<ui::standard> const &, project_lifetime_id const &);

    project_lifetime_id const project_lifetime_id;

    std::shared_ptr<ui::standard> const standard;
    std::shared_ptr<ui::texture> const texture;
    std::shared_ptr<ui::font_atlas> const normal_font_atlas;
    std::shared_ptr<ui::font_atlas> const time_font_atlas;
    std::shared_ptr<ui_atlas> const atlas;
    std::shared_ptr<ui_square_mesh_data> const square_mesh_data;
    std::shared_ptr<ae::modifiers_holder> const modifiers_holder;
    std::shared_ptr<ae::keyboard> const keyboard;

    std::shared_ptr<ui_base_lifecycle> const base_lifecycle;

   private:
    ui_resource_lifetime(ui_resource_lifetime const &) = delete;
    ui_resource_lifetime(ui_resource_lifetime &&) = delete;
    ui_resource_lifetime &operator=(ui_resource_lifetime const &) = delete;
    ui_resource_lifetime &operator=(ui_resource_lifetime &&) = delete;
};
}  // namespace yas::ae
