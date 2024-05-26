//
//  ui_background.cpp
//

#include "ui_background.hpp"
#include <ae-core/app/features/color.h>
#include <ae-core/global/hierarchy.h>
#include <ae-core/global/ui_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_background> ui_background::make_shared(project_lifetime_id const &lifetime_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(lifetime_id);

    return std::make_shared<ui_background>(resource_lifetime->standard, app_lifetime->color.get());
}

ui_background::ui_background(std::shared_ptr<ui::standard> const &standard, ae::color *color)
    : _background(standard->view_look()->background().get()), _color(color) {
    standard->view_look()
        ->observe_appearance([this](auto const &) { this->_background->set_color(this->_color->background()); })
        .sync()
        ->add_to(this->_pool);
}
