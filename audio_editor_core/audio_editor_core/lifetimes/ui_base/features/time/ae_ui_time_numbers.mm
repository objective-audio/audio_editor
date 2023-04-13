//
//  ae_ui_time_numbers.cpp
//

#include "ae_ui_time_numbers.hpp"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_time_numbers_presenter.h>
#include <audio_editor_core/ae_ui_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_time_numbers> ui_time_numbers::make_shared(window_lifetime_id const &lifetime_id,
                                                              std::shared_ptr<ui::node> const &node) {
    auto const presenter = time_numbers_presenter::make_shared(lifetime_id);
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(lifetime_id);

    return std::make_shared<ui_time_numbers>(presenter, resource_lifetime->time_font_atlas, resource_lifetime->standard,
                                             app_lifetime->color.get(), node);
}

ui_time_numbers::ui_time_numbers(std::shared_ptr<time_numbers_presenter> const &presenter,
                                 std::shared_ptr<ui::font_atlas> const &font_atlas,
                                 std::shared_ptr<ui::standard> const &standard, ae::color *color,
                                 std::shared_ptr<ui::node> const &node)
    : _presenter(presenter),
      _strings(ui::strings::make_shared({.alignment = ui::layout_alignment::mid}, font_atlas)),
      _color(color) {
    node->add_sub_node(this->_strings->rect_plane()->node());

    this->_strings->rect_plane()->node()->meshes().at(0)->set_use_mesh_color(true);

    presenter->observe_editing_time_text_range([this](auto const &) { this->_update_time_strings_attributes(); })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) { this->_update_time_strings_attributes(); })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) { this->_strings->set_text(this->_presenter->time_text()); })
        .end()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::layout_region_source> ui_time_numbers::actual_layout_source() const {
    return this->_strings->actual_layout_source();
}

std::optional<ui::region> ui_time_numbers::button_region(index_range const &range) const {
    auto const &time_strings = this->_strings;

    std::optional<ui::region> region = std::nullopt;

    auto each = make_fast_each(range.index, range.index + range.length);
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &cell_regions = time_strings->actual_cell_regions();

        if (idx < cell_regions.size()) {
            auto const &cell_region = cell_regions.at(idx);

            if (region.has_value()) {
                region = region.value().combined(cell_region);
            } else {
                region = cell_region;
            }
        }
    }

    return region;
}

void ui_time_numbers::_update_time_strings_attributes() {
    auto const editing_unit_idx = this->_presenter->editing_unit_index();
    auto const ranges = this->_presenter->time_text_unit_ranges();

    std::vector<ui::strings_attribute> attributes;
    attributes.emplace_back(ui::strings_attribute{.range = std::nullopt, .color = this->_color->time_text()});

    auto each = make_fast_each(ranges.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);

        if (idx == editing_unit_idx) {
            attributes.emplace_back(
                ui::strings_attribute{.range = ranges.at(idx), .color = this->_color->time_selected_text()});
        }
    }

    this->_strings->set_attributes(std::move(attributes));
}

observing::syncable ui_time_numbers::observe_button_regions(std::function<void(void)> &&handler) {
    return this->_strings->observe_actual_cell_regions([handler = std::move(handler)](auto const &) { handler(); });
}
