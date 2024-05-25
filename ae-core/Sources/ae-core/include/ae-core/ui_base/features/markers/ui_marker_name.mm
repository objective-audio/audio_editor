//
//  ui_marker_name.cpp
//

#include "ui_marker_name.h"
#include <ae-core/app/features/color.h>
#include <ae-core/global/ui_hierarchy.h>
#include <ae-core/ui_base/features/markers/ui_marker_constants.h>
#include <cpp-utils/assertion.h>
#include <ae-core/ui_base/features/common/ui_atlas.hpp>

using namespace yas;
using namespace yas::ae;

std::unique_ptr<ui_marker_name> ui_marker_name::make_unique(project_lifetime_id const &lifetime_id,
                                                            ui::node *parent_node) {
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(lifetime_id);

    return std::unique_ptr<ui_marker_name>(new ui_marker_name{resource_lifetime->normal_font_atlas, parent_node});
}

ui_marker_name::ui_marker_name(std::shared_ptr<ui::font_atlas> const &font_atlas, ui::node *parent_node)
    : _name_strings(ui::strings::make_shared(
          {.frame = ui::region{.origin = ui::point::zero(),
                               .size = {0.0f, -static_cast<float>(font_atlas->ascent() + font_atlas->descent())}}},
          font_atlas)) {
    parent_node->add_sub_node(this->node());
}

std::shared_ptr<ui::node> const &ui_marker_name::node() const {
    return this->_name_strings->rect_plane()->node();
}

void ui_marker_name::set_content(marker_content const &content) {
    this->_content = std::nullopt;

    this->update_content(content);
}

void ui_marker_name::update_content(marker_content const &content) {
    if (this->_content.has_value() && this->_content.value().identifier != content.identifier) {
        assertion_failure_if_not_test();
    }

    this->_content = content;

    auto const &node = this->node();
    node->set_is_enabled(true);
    node->set_x(content.x());

    this->_name_strings->set_text(content.name);
}

void ui_marker_name::reset_content() {
    this->_cancellable = nullptr;
    this->_content = std::nullopt;
    this->node()->set_is_enabled(false);
}

ui::region ui_marker_name::square_region() const {
    if (!this->_content.has_value()) {
        return ui::region::zero();
    }

    auto const region = this->_name_strings->actual_frame();
    auto const &font_atlas = this->_name_strings->font_atlas();
    float const font_height = font_atlas->ascent() + font_atlas->descent();

    return ui::region{.origin = {.x = this->_content.value().x()},
                      .size = {.width = std::max(region.size.width, ui_marker_constants::min_square_width),
                               .height = -std::max(region.size.height, font_height)}};
}

void ui_marker_name::finalize() {
    this->node()->remove_from_super_node();
}

void ui_marker_name::update_color(ui::color const &selected_color, ui::color const &normal_color) {
    if (this->_content.has_value()) {
        bool const is_selected = this->_content.value().is_selected;
        auto const &text_color = is_selected ? selected_color : normal_color;

        this->node()->set_color(text_color);
    }
}
