//
//  ae_ui_time_buttons.mm
//

#include "ae_ui_time_buttons.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <audio_editor_core/ae_time_numbers_presenter.h>
#include <audio_editor_core/ae_ui_button_utils.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_time_constants.h>
#include <audio_editor_core/ae_ui_types.h>
#include <cpp_utils/yas_fast_each.h>
#include <audio_editor_core/ae_ui_time_numbers.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_time_buttons> ui_time_buttons::make_shared(window_lifetime_id const &window_lifetime_id,

                                                              ui::node *node, ui_time_numbers *time_numbers) {
    auto const presenter = time_numbers_presenter::make_shared(window_lifetime_id);

    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(window_lifetime_id);

    return std::make_shared<ui_time_buttons>(resource_lifetime->standard, app_lifetime->color.get(), presenter,
                                             project_lifetime->action_sender, node, time_numbers);
}

ui_time_buttons::ui_time_buttons(std::shared_ptr<ui::standard> const &standard, ae::color *color,
                                 std::shared_ptr<time_numbers_presenter> const &presenter,
                                 std::shared_ptr<project_action_sender> const &action_sender, ui::node *node,
                                 ui_time_numbers *time_numbers)
    : _presenter(presenter),
      _action_sender(action_sender),
      _standard(standard),
      _color(color),
      _node(node),
      _time_numbers(time_numbers) {
    // 現状ではUnitの個数が変わらないので初期化時だけ呼び出す
    this->_resize_buttons();

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

            button->set_can_begin_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}));
            button->set_can_indicate_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}));

            button->rect_plane()->node()->mesh()->set_use_mesh_color(true);

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
