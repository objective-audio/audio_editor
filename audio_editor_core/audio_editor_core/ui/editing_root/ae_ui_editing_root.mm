//
//  ae_ui_editing_root.cpp
//

#include "ae_ui_editing_root.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_editing_root_presenter.h>
#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_keyboard.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <audio_editor_core/ae_ui_editing_root_utils.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_modal_bg.h>
#include <audio_editor_core/ae_ui_root_lifetime.h>
#include <audio_editor_core/ae_ui_scroller.h>
#include <audio_editor_core/ae_ui_time.h>
#include <audio_editor_core/ae_ui_track.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_editing_root> ui_editing_root::make_shared(window_lifetime_id const &window_lifetime_id,
                                                              std::shared_ptr<ui::standard> const &standard,
                                                              std::shared_ptr<ui::font_atlas> const &font_atlas,
                                                              std::shared_ptr<ae::keyboard> const &keyboard,
                                                              std::shared_ptr<ui_scroller> const &ui_scroller,
                                                              std::shared_ptr<ui_modal_bg> const &ui_modal_bg,
                                                              std::shared_ptr<ui_time> const &ui_time) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);

    auto const presenter = editing_root_presenter::make_shared(window_lifetime_id);

    return std::make_shared<ui_editing_root>(
        standard, font_atlas, app_lifetime->color, presenter, project_lifetime->action_sender,
        project_lifetime->pinch_gesture_controller, keyboard, ui_scroller, ui_modal_bg, ui_time);
}

ui_editing_root::ui_editing_root(std::shared_ptr<ui::standard> const &standard,
                                 std::shared_ptr<ui::font_atlas> const &font_atlas,
                                 std::shared_ptr<ae::color> const &color,
                                 std::shared_ptr<editing_root_presenter> const &presenter,
                                 std::shared_ptr<project_action_sender> const &action_sender,
                                 std::shared_ptr<pinch_gesture_controller> const &pinch_gesture_controller,
                                 std::shared_ptr<ae::keyboard> const &keyboard,
                                 std::shared_ptr<ui_scroller> const &scroller,
                                 std::shared_ptr<ui_modal_bg> const &modal_bg, std::shared_ptr<ui_time> const &time)
    : node(ui::node::make_shared()),
      _presenter(presenter),
      _action_sender(action_sender),
      _pinch_gesture_controller(pinch_gesture_controller),
      _color(color),
      _playing_line(ui::rect_plane::make_shared(1)),
      _scroller(scroller),
      _modal_bg(modal_bg),
      _time(time) {
    this->_setup_node_hierarchie();
    this->_setup_observing(standard, keyboard);
    this->_setup_layout(standard->view_look());
}

bool ui_editing_root::responds_to_action(action const action) {
    return this->_presenter->responds_to_action(action);
}

void ui_editing_root::_setup_node_hierarchie() {
    this->node->add_sub_node(this->_scroller->node);

    this->node->add_sub_node(this->_playing_line->node());
    this->node->add_sub_node(this->_modal_bg->node);
    this->node->add_sub_node(this->_time->node);
}

void ui_editing_root::_setup_observing(std::shared_ptr<ui::standard> const &standard,
                                       std::shared_ptr<ae::keyboard> const &keyboard) {
    standard->renderer()
        ->observe_will_render([this](auto const &) {
            this->_playing_line->node()->set_color(
                ui_editing_root_utils::to_playing_line_color(this->_presenter->playing_line_state(), this->_color));
        })
        .end()
        ->add_to(this->_pool);

    keyboard
        ->observe_key([this](ae::key const &key) {
            if (auto const action_sender = this->_action_sender.lock()) {
                action_sender->send(key);
            }
        })
        .end()
        ->add_to(this->_pool);
    keyboard
        ->observe_modifier([this](ae::modifier_event const &event) {
            switch (event.modifier) {
                case ae::modifier::shift:
                    if (auto const controller = this->_pinch_gesture_controller.lock()) {
                        controller->handle_modifier(event.state);
                    }
                    break;

                default:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);

    standard->event_manager()
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            if (event->type() == ui::event_type::pinch) {
                auto const &pinch_event = event->get<ui::pinch>();
                gesture_state const state = to_gesture_state(event->phase());

                if (auto const controller = this->_pinch_gesture_controller.lock()) {
                    controller->handle_gesture(
                        pinch_gesture{.state = state, .magnification = pinch_event.magnification});
                }
            }
        })
        .end()
        ->add_to(this->_pool);
}

void ui_editing_root::_setup_layout(std::shared_ptr<ui::view_look> const &view_look) {
    auto const &safe_area_guide = view_look->safe_area_layout_guide();

    // playing_line

    safe_area_guide
        ->observe([this](ui::region const &src_region) {
            ui::region const dst_region{.origin = {.x = src_region.center().x - 0.5f, .y = src_region.origin.y},
                                        .size = {.width = 1.0f, .height = src_region.size.height}};
            this->_playing_line->data()->set_rect_position(dst_region, 0);
        })
        .sync()
        ->add_to(this->_pool);
}
