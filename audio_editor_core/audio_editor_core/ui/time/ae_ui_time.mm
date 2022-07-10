//
//  ae_ui_time.mm
//

#include "ae_ui_time.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_action_controller.h>
#include <audio_editor_core/ae_time_presenter.h>
#include <audio_editor_core/ae_ui_button_utils.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_root_lifetime.h>
#include <audio_editor_core/ae_ui_time_constants.h>
#include <audio_editor_core/ae_ui_types.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_time> ui_time::make_shared(window_lifetime_id const &window_lifetime_id,
                                              std::shared_ptr<ui::standard> const &standard,
                                              std::shared_ptr<ui::texture> const &texture) {
    auto const presenter = time_presenter::make_shared(window_lifetime_id);

    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);

    return std::make_shared<ui_time>(standard, texture, app_lifetime->color, presenter,
                                     project_lifetime->action_controller);
}

ui_time::ui_time(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ui::texture> const &texture,
                 std::shared_ptr<ae::color> const &color, std::shared_ptr<time_presenter> const &presenter,
                 std::shared_ptr<project_action_controller> const &action_controller)
    : node(ui::node::make_shared()),
      _presenter(presenter),
      _action_controller(action_controller),
      _standard(standard),
      _color(color),
      _font_atlas(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold", .font_size = 26.0f, .words = " 1234567890.:+-"}, texture)),
      _top_guide(ui::layout_value_guide::make_shared()),
      _bg_button(ui::button::make_shared(ui::region{.size = {1.0f, 1.0f}}, standard)),
      _buttons_root_node(ui::node::make_shared()),
      _nudge_plane(ui::rect_plane::make_shared(1)),
      _time_strings(ui::strings::make_shared({.alignment = ui::layout_alignment::mid}, _font_atlas)) {
    this->node->add_sub_node(this->_bg_button->rect_plane()->node());
    this->node->add_sub_node(this->_buttons_root_node);
    this->node->add_sub_node(this->_nudge_plane->node());
    this->node->add_sub_node(this->_time_strings->rect_plane()->node());

    this->node->set_batch(ui::batch::make_shared());

    auto const &bg_plane = this->_bg_button->rect_plane();
    bg_plane->node()->mesh()->set_use_mesh_color(true);

    this->_bg_button->set_can_begin_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}));
    this->_bg_button->set_can_indicate_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}));

    this->_bg_button
        ->observe([this](ui::button::context const &context) {
            switch (context.phase) {
                case ui::button::phase::ended:
                    if (context.touch.touch_id == ui::touch_id::mouse_left()) {
                        if (auto const action_controller = this->_action_controller.lock()) {
                            action_controller->send(action_kind::begin_time_editing, "");
                        }
                    }
                    break;
                default:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);

    this->_resize_buttons();

    this->_nudge_plane->data()->set_rect_position(ui::region{.size = {1.0f, 1.0f}}, 0);

    float const node_y_offset =
        ui_time_constants::nudge_height + this->_font_atlas->ascent() + this->_font_atlas->descent();
    ui::layout(standard->view_look()->view_layout_guide()->bottom(), this->_top_guide,
               ae::ui_layout_utils::constant(node_y_offset))
        .sync()
        ->add_to(this->_pool);
    this->node->attach_y_layout_guide(*this->_top_guide);

    this->_time_strings->rect_plane()->node()->mesh()->set_use_mesh_color(true);

    this->_time_strings->actual_layout_source()
        ->observe_layout_region([this](ui::region const &region) {
            auto const &bg_button_node = this->_bg_button->rect_plane()->node();
            float const nudge_height = ui_time_constants::nudge_height;
            bg_button_node->set_position(region.origin - ui::point{.x = 0.0f, .y = nudge_height});
            bg_button_node->set_scale({.width = region.size.width, .height = region.size.height + nudge_height});
        })
        .sync()
        ->add_to(this->_pool);

    this->_time_strings
        ->observe_actual_cell_regions([this](const std::vector<ui::region> &cell_regions) {
            this->_update_button_positions();
            this->_update_nudge_position();
        })
        .end()
        ->add_to(this->_pool);

    this->_presenter
        ->observe_editing_time_text_range([this](auto const &) {
            this->_update_unit_states();
            this->_update_time_strings_attributes();
        })
        .sync()
        ->add_to(this->_pool);

    this->_presenter->observe_nudging_unit_index([this](std::size_t const &) { this->_update_nudge_position(); })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) { this->_time_strings->set_text(this->_presenter->time_text()); })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) {
            auto const &color = this->_color;
            auto const &bg_data = this->_bg_button->rect_plane()->data();
            bg_data->set_rect_color(this->_color->time_bg(), to_rect_index(0, false));
            bg_data->set_rect_color(this->_color->time_bg_tracking(), to_rect_index(0, true));

            this->_nudge_plane->node()->set_color(color->time_nudging_line());

            for (auto const &element : this->_button_elements) {
                auto const &data = element.button->rect_plane()->data();
                data->set_rect_color(this->_color->time_unit(), to_rect_index(0, false));
                data->set_rect_color(this->_color->time_unit_tracking(), to_rect_index(0, true));
                data->set_rect_color(this->_color->time_unit_selected(), to_rect_index(1, false));
                data->set_rect_color(this->_color->time_unit_selected_tracking(), to_rect_index(1, true));
            }

            this->_update_time_strings_attributes();
        })
        .sync()
        ->add_to(this->_pool);
}

void ui_time::_resize_buttons() {
    auto const standard = this->_standard.lock();
    if (!standard) {
        return;
    }

    auto const size = this->_presenter->time_text_unit_ranges().size();

    if (size < this->_button_elements.size()) {
        auto each = make_fast_each(size, this->_button_elements.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            this->_button_elements.at(idx).button->rect_plane()->node()->remove_from_super_node();
        }

        this->_button_elements.resize(size);
    } else if (this->_button_elements.size() < size) {
        auto const adding_count = size - this->_button_elements.size();
        auto each = make_fast_each(adding_count);
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto button =
                ui::button::make_shared(ui::region{.origin = ui::point::zero(), .size = {1.0f, 1.0f}}, standard, 2);

            button->set_can_begin_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}));
            button->set_can_indicate_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}));

            button->rect_plane()->node()->mesh()->set_use_mesh_color(true);

            auto canceller =
                button
                    ->observe([this, idx](ui::button::context const &context) {
                        switch (context.phase) {
                            case ui::button::phase::ended: {
                                // 左クリック
                                if (context.touch.touch_id == ui::touch_id::mouse_left()) {
                                    if (auto const action_controller = this->_action_controller.lock()) {
                                        action_controller->send(action_kind::select_time_unit, std::to_string(idx));
                                    }
                                }
                            } break;
                            default:
                                break;
                        }
                    })
                    .end();

            this->_buttons_root_node->add_sub_node(button->rect_plane()->node());

            this->_button_elements.emplace_back(
                button_element{.button = std::move(button), .canceller = std::move(canceller)});
        }
    }
}

void ui_time::_update_button_positions() {
    auto const ranges = this->_presenter->time_text_unit_ranges();

    auto each = make_fast_each(ranges.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);

        if (idx >= this->_button_elements.size()) {
            break;
        }

        auto const &node = this->_button_elements.at(idx).button->rect_plane()->node();
        auto const &range = ranges.at(idx);

        if (auto const region = this->_button_region(range)) {
            auto const &region_value = region.value();
            node->set_position(region_value.origin);
            node->set_scale(region_value.size);
            node->set_is_enabled(true);
        } else {
            node->set_is_enabled(false);
        }
    }
}

std::optional<ui::region> ui_time::_button_region(index_range const &range) const {
    std::optional<ui::region> region = std::nullopt;

    auto each = make_fast_each(range.index, range.index + range.length);
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &cell_regions = this->_time_strings->actual_cell_regions();

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

void ui_time::_update_unit_states() {
    auto const editing_unit_idx = this->_presenter->editing_unit_index();
    auto const ranges = this->_presenter->time_text_unit_ranges();

    auto each = make_fast_each(this->_button_elements.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &button = this->_button_elements.at(idx).button;

        if (idx == editing_unit_idx && idx < ranges.size()) {
            button->set_state_index(1);
        } else {
            button->set_state_index(0);
        }
    }
}

void ui_time::_update_time_strings_attributes() {
    auto const editing_unit_idx = this->_presenter->editing_unit_index();
    auto const ranges = this->_presenter->time_text_unit_ranges();

    std::vector<ui::strings_attribute> attributes;
    attributes.emplace_back(ui::strings_attribute{.range = std::nullopt, .color = this->_color->time_text()});

    auto each = make_fast_each(this->_button_elements.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);

        if (idx == editing_unit_idx && idx < ranges.size()) {
            attributes.emplace_back(
                ui::strings_attribute{.range = ranges.at(idx), .color = this->_color->time_selected_text()});
        }
    }

    this->_time_strings->set_attributes(std::move(attributes));
}

void ui_time::_update_nudge_position() {
    auto const nudging_unit_idx = this->_presenter->nudging_unit_index();
    auto const &node = this->_nudge_plane->node();

    if (!nudging_unit_idx.has_value()) {
        node->set_is_enabled(false);
        return;
    }

    auto const &idx = nudging_unit_idx.value();
    auto const ranges = this->_presenter->time_text_unit_ranges();

    if (idx < ranges.size()) {
        auto const &range = ranges.at(idx);

        auto const region = this->_button_region(range);

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
