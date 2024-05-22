//
//  ae_ui_time_nudge.cpp
//

#include "ae_ui_time_nudge.hpp"
#include <ae-core/app/features/ae_color.h>
#include <ae-core/global/ae_ui_hierarchy.h>
#include <ae-core/ui_base/features/time/ae_ui_time_constants.h>
#include <ae-core/ui_base/features/time/time_numbers_presenter/ae_time_numbers_presenter.h>
#include <ae-core/ui_base/features/time/ae_ui_time_numbers.hpp>
#include <ae-core/ui_base/features/time/time_nudge_presenter/ae_time_nudge_presenter.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_time_nudge> ui_time_nudge::make_shared(project_lifetime_id const &lifetime_id,
                                                          ui_time_numbers *time_numbers,
                                                          std::shared_ptr<ui::node> const &node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(lifetime_id);
    auto const unit_presenter = time_numbers_presenter::make_shared(lifetime_id);
    auto const nudge_preseneter = time_nudge_presenter::make_shared();

    return std::make_shared<ui_time_nudge>(app_lifetime->color.get(), resource_lifetime->standard->view_look(),
                                           time_numbers, unit_presenter, nudge_preseneter, node);
}

ui_time_nudge::ui_time_nudge(ae::color *color, std::shared_ptr<ui::view_look> const &view_look,
                             ui_time_numbers *time_numbers, std::shared_ptr<time_numbers_presenter> const &presenter,
                             std::shared_ptr<time_nudge_presenter> const &nudge_presenter,
                             std::shared_ptr<ui::node> const &node)
    : _color(color),
      _time_numbers(time_numbers),
      _nudge_plane(ui::rect_plane::make_shared(1)),
      _unit_presenter(presenter),
      _nudge_presenter(nudge_presenter) {
    node->add_sub_node(this->_nudge_plane->node());

    this->_nudge_plane->data()->set_rect_position(ui::region{.size = {1.0f, 1.0f}}, 0);

    time_numbers->observe_button_regions([this] { this->_update_nudge_position(); }).end()->add_to(this->_pool);

    this->_nudge_presenter
        ->observe_nudging_unit_kind([this](timing_unit_kind const &) { this->_update_nudge_position(); })
        .sync()
        ->add_to(this->_pool);

    view_look
        ->observe_appearance(
            [this](auto const &) { this->_nudge_plane->node()->set_color(this->_color->time_nudging_line()); })
        .sync()
        ->add_to(this->_pool);
}

void ui_time_nudge::_update_nudge_position() {
    auto const nudging_unit_idx = this->_nudge_presenter->nudging_unit_index();
    auto const &node = this->_nudge_plane->node();

    if (!nudging_unit_idx.has_value()) {
        node->set_is_enabled(false);
        return;
    }

    auto const &idx = nudging_unit_idx.value();
    auto const ranges = this->_unit_presenter->time_text_unit_ranges();

    if (idx < ranges.size()) {
        auto const &range = ranges.at(idx);

        auto const region = this->_time_numbers->button_region(range);

        if (region.has_value()) {
            auto const &region_value = region.value();
            float const height = ui_time_constants::nudge_height;
            node->set_position(ui::point{.x = region_value.left(), .y = region_value.bottom() - height});
            node->set_scale(ui::size{.width = region_value.size.width, .height = height});
            node->set_is_enabled(true);
        } else {
            node->set_is_enabled(false);
        }
    } else {
        node->set_is_enabled(false);
    }
}
