//
//  ae_ui_range_selection.mm
//

#include "ae_ui_range_selection.hpp"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_range_selector.hpp>
#include <audio_editor_core/ae_ui_framed_square.hpp>
#include <audio_editor_core/ae_ui_square_mesh_data.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_range_selection> ui_range_selection::make_shared(project_lifetime_id const &project_lifetime_id,
                                                                    ui::node *node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);
    return std::make_shared<ui_range_selection>(node, app_lifetime->color.get(), resource_lifetime->standard.get(),
                                                resource_lifetime->square_mesh_data.get(),
                                                project_lifetime->range_selector);
}

ui_range_selection::ui_range_selection(ui::node *node, ae::color *color, ui::standard *standard,
                                       ui_square_mesh_data *square_mesh_data,
                                       std::shared_ptr<range_selector> const &range_selector)
    : _node(node),
      _color(color),
      _framed_square(std::make_shared<ui_framed_square>(square_mesh_data)),
      _range_selector(range_selector) {
    node->add_sub_node(this->_framed_square->node);

    range_selector
        ->observe_region([this](const std::optional<ui::region> &region) {
            this->_framed_square->node->set_is_enabled(region.has_value());

            if (region.has_value()) {
                auto const insets = region.value().insets();
                auto const min = this->_framed_square->node->convert_position({.x = insets.left, .y = insets.bottom});
                auto const max = this->_framed_square->node->convert_position({.x = insets.right, .y = insets.top});
                this->_framed_square->set_region(
                    {.origin = {.x = min.x, .y = min.y}, .size = {.width = max.x - min.x, .height = max.y - min.y}});
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](ui::appearance const &) {
            this->_framed_square->set_fill_color(this->_color->range_selection_fill());
            this->_framed_square->set_frame_color(this->_color->range_selection_frame());
        })
        .sync()
        ->add_to(this->_pool);
}
