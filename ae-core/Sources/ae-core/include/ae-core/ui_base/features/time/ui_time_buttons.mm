//
//  ui_time_buttons.mm
//

#include "ui_time_buttons.h"
#include <ae-core/app/features/color.h>
#include <ae-core/global/hierarchy.h>
#include <ae-core/global/ui_hierarchy.h>
#include <ae-core/project_editing/features/project_action_sender.h>
#include <ae-core/ui_base/features/time/time_numbers_presenter/time_numbers_presenter.h>
#include <ae-core/ui_base/features/time/ui_time_constants.h>
#include <ae-core/ui_base/utils/ui_button_utils.h>
#include <ae-core/ui_base/utils/ui_layout_utils.h>
#include <ae-core/ui_base/value_types/ui_types.h>
#include <cpp-utils/fast_each.h>
#include <ae-core/ui_base/features/common/ui_atlas.hpp>
#include <ae-core/ui_base/features/time/ui_time_numbers.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_time_buttons> ui_time_buttons::make_shared(project_lifetime_id const &project_lifetime_id,

                                                              ui::node *node, ui_time_numbers *time_numbers) {
    auto const presenter = time_numbers_presenter::make_shared(project_lifetime_id);

    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);

    return std::make_shared<ui_time_buttons>(resource_lifetime->standard, app_lifetime->color.get(), presenter,
                                             project_editing_lifetime->action_sender, node, time_numbers,
                                             resource_lifetime->atlas.get());
}

ui_time_buttons::ui_time_buttons(std::shared_ptr<ui::standard> const &standard, ae::color *color,
                                 std::shared_ptr<time_numbers_presenter> const &presenter,
                                 std::shared_ptr<project_action_sender> const &action_sender, ui::node *node,
                                 ui_time_numbers *time_numbers, ui_atlas const *atlas)
    : _presenter(presenter),
      _action_sender(action_sender),
      _standard(standard),
      _color(color),
      _node(node),
      _time_numbers(time_numbers),
      _atlas(atlas) {
    // 現状ではUnitの個数が変わらないので初期化時だけ呼び出す
    this->_resize_buttons();

    atlas
        ->observe_white_filled_tex_coords([this](ui::uint_region const &tex_coords) {
            for (auto const &element : this->_button_elements) {
                auto const &button_data = element.button->rect_plane()->data();
                auto each = make_fast_each(button_data->rect_count());
                while (yas_each_next(each)) {
                    button_data->set_rect_tex_coords(tex_coords, yas_each_index(each));
                }
            }
        })
        .sync()
        ->add_to(this->_pool);

    time_numbers->observe_button_regions([this] { this->_update_button_positions(); }).end()->add_to(this->_pool);

    this->_presenter->observe_editing_time_text_range([this](auto const &) { this->_update_unit_states(); })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) {
            for (auto const &element : this->_button_elements) {
                auto const &data = element.button->rect_plane()->data();
                data->set_rect_color(this->_color->time_unit(), to_rect_index(0, false));
                data->set_rect_color(this->_color->time_unit_tracking(), to_rect_index(0, true));
                data->set_rect_color(this->_color->time_unit_selected(), to_rect_index(1, false));
                data->set_rect_color(this->_color->time_unit_selected_tracking(), to_rect_index(1, true));
            }
        })
        .sync()
        ->add_to(this->_pool);
}

void ui_time_buttons::_resize_buttons() {
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

            button->set_can_begin_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}, true));
            button->set_can_indicate_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}, true));

            auto const &mesh = button->rect_plane()->node()->meshes().at(0);
            mesh->set_use_mesh_color(true);
            mesh->set_texture(this->_atlas->texture());

            auto canceller = button
                                 ->observe([this, idx](ui::button::context const &context) {
                                     switch (context.phase) {
                                         case ui::button::phase::ended: {
                                             // 左クリック
                                             if (context.touch.touch_id == ui::touch_id::mouse_left()) {
                                                 if (auto const action_sender = this->_action_sender.lock()) {
                                                     action_sender->send(editing_action_name::begin_time_editing);
                                                     action_sender->send(time_editing_action_name::select_time_unit,
                                                                         static_cast<int64_t>(idx));
                                                 }
                                             }
                                         } break;
                                         default:
                                             break;
                                     }
                                 })
                                 .end();

            this->_node->add_sub_node(button->rect_plane()->node());

            this->_button_elements.emplace_back(
                button_element{.button = std::move(button), .canceller = std::move(canceller)});
        }
    }
}

void ui_time_buttons::_update_button_positions() {
    auto const ranges = this->_presenter->time_text_unit_ranges();

    auto each = make_fast_each(ranges.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);

        if (idx >= this->_button_elements.size()) {
            break;
        }

        auto const &node = this->_button_elements.at(idx).button->rect_plane()->node();
        auto const &range = ranges.at(idx);

        if (auto const region = this->_time_numbers->button_region(range)) {
            auto const &region_value = region.value();
            node->set_position(region_value.origin);
            node->set_scale(region_value.size);
            node->set_is_enabled(true);
        } else {
            node->set_is_enabled(false);
        }
    }
}

void ui_time_buttons::_update_unit_states() {
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
