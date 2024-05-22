//
//  ae_ui_resource_lifetime.cpp
//

#include "ae_ui_resource_lifetime.hpp"
#include <ae-core/project/features/ae_display_space.h>
#include <ae-core/ui_base/features/common/ae_ui_mesh_data.h>
#include <ae-core/ui_resource/features/ae_keyboard.h>
#include <ae-core/ui_base/features/common/ae_ui_atlas.hpp>
#include <ae-core/ui_resource/features/ae_modifiers_holder.hpp>
#include <ae-core/ui_resource/features/ae_ui_square_mesh_data.hpp>
#include <ae-core/ui_resource/lifecycles/ae_ui_base_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

ui_resource_lifetime::ui_resource_lifetime(std::shared_ptr<ui::standard> const &standard,
                                           ae::project_lifetime_id const &lifetime_id)
    : standard(standard),
      project_lifetime_id(lifetime_id),
      texture(ui::texture::make_shared({.point_size = {1024, 1024}}, standard->view_look())),
      normal_font_atlas(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold",
           .font_size = 14.0f,
           .words = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:[]"},
          this->texture)),
      time_font_atlas(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold", .font_size = 26.0f, .words = " 1234567890.:+-"}, this->texture)),
      atlas(std::make_shared<ui_atlas>(this->texture)),
      square_mesh_data(std::make_shared<ui_square_mesh_data>(this->atlas.get())),
      modifiers_holder(std::make_shared<ae::modifiers_holder>(standard->event_manager())),
      keyboard(std::make_shared<ae::keyboard>(standard->event_manager(), this->modifiers_holder.get())),
      base_lifecycle(std::make_shared<ae::ui_base_lifecycle>(lifetime_id)) {
}
