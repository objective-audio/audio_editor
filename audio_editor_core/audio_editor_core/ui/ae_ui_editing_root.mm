//
//  ae_ui_root.cpp
//

#include "ae_ui_editing_root.h"
#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_editing_root_presenter.h>
#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_keyboard.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>
#include <audio_editor_core/ae_ui_editing_root_utils.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_time.h>
#include <audio_editor_core/ae_ui_track.h>

using namespace yas;
using namespace yas::ae;

ui_editing_root::ui_editing_root(std::shared_ptr<ui::standard> const &standard,
                                 std::shared_ptr<ui::texture> const &texture,
                                 std::shared_ptr<editing_root_presenter> const &presenter,
                                 std::shared_ptr<action_controller> const &action_controller,
                                 std::shared_ptr<pinch_gesture_controller> const &pinch_gesture_controller,
                                 std::shared_ptr<ui_track> const &track, std::shared_ptr<ui_time> const &time)
    : _presenter(presenter),
      _action_controller(action_controller),
      _pinch_gesture_controller(pinch_gesture_controller),
      _standard(standard),
      _keyboard(ae::keyboard::make_shared(standard->event_manager())),
      _font_atlas(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold",
           .font_size = 14.0f,
           .words = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:[]"},
          texture)),
      _status_strings(ui::strings::make_shared(
          {.text = "", .alignment = ui::layout_alignment::min, .max_word_count = 128}, this->_font_atlas)),
      _file_info_strings(ui::strings::make_shared(
          {.text = "", .alignment = ui::layout_alignment::min, .max_word_count = 128}, this->_font_atlas)),
      _file_track_strings(ui::strings::make_shared(
          {.text = "", .alignment = ui::layout_alignment::min, .max_word_count = 1024}, this->_font_atlas)),
      _marker_pool_strings(ui::strings::make_shared(
          {.text = "", .alignment = ui::layout_alignment::min, .max_word_count = 1024}, this->_font_atlas)),
      _track(track),
      _playing_line(ui::rect_plane::make_shared(1)),
      _time(time) {
    standard->view_look()->background()->set_rgb_color({.v = 0.2f});

    this->_file_info_strings->set_text(presenter->file_info_text());

    this->_setup_node_hierarchie();
    this->_setup_observing();
    this->_setup_layout();
}

bool ui_editing_root::responds_to_action(action const action) {
    return this->_presenter->responds_to_action(action);
}

void ui_editing_root::_setup_node_hierarchie() {
    auto const &root_node = this->_standard->root_node();

    root_node->add_sub_node(this->_track->node());

    root_node->add_sub_node(this->_status_strings->rect_plane()->node());
    root_node->add_sub_node(this->_file_info_strings->rect_plane()->node());
    root_node->add_sub_node(this->_file_track_strings->rect_plane()->node());
    root_node->add_sub_node(this->_marker_pool_strings->rect_plane()->node());

    root_node->add_sub_node(this->_playing_line->node());
    root_node->add_sub_node(this->_time->node());
}

void ui_editing_root::_setup_observing() {
    auto const &presenter = this->_presenter;

    presenter->observe_state_text([this](std::string const &string) { this->_status_strings->set_text(string); })
        .sync()
        ->add_to(this->_pool);

    presenter
        ->observe_file_track_text([this](std::string const &string) { this->_file_track_strings->set_text(string); })
        .sync()
        ->add_to(this->_pool);

    presenter
        ->observe_marker_pool_text([this](std::string const &string) { this->_marker_pool_strings->set_text(string); })
        .sync()
        ->add_to(this->_pool);

    this->_standard->renderer()
        ->observe_will_render([this](auto const &) {
            this->_playing_line->node()->set_rgb_color(
                ui_editing_root_utils::to_playing_line_color(this->_presenter->playing_line_state()));
        })
        .end()
        ->add_to(this->_pool);

    this->_keyboard
        ->observe_key([this](ae::key const &key) {
            if (auto const controller = this->_action_controller.lock()) {
                controller->handle_key(key);
            }
        })
        .end()
        ->add_to(this->_pool);
    this->_keyboard
        ->observe_modifier([this](ae::modifier_event const &event) {
            switch (event.modifier) {
                case ae::modifier::shift:
                    this->_pinch_gesture_controller->handle_modifier(event.state);
                    break;

                default:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);

    this->_standard->event_manager()
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            if (event->type() == ui::event_type::pinch) {
                auto const &pinch_event = event->get<ui::pinch>();
                gesture_state const state = to_gesture_state(event->phase());

                this->_pinch_gesture_controller->handle_gesture(
                    pinch_gesture{.state = state, .magnification = pinch_event.magnification()});
            }
        })
        .end()
        ->add_to(this->_pool);
}

void ui_editing_root::_setup_layout() {
    auto const &safe_area_guide = this->_standard->view_look()->safe_area_layout_guide();
    auto const &safe_area_h_guide = this->_standard->view_look()->safe_area_layout_guide()->horizontal_range();
    auto const &status_preferred_guide = this->_status_strings->preferred_layout_guide();
    auto const status_actual_source = this->_status_strings->actual_layout_source();
    auto const &file_info_preferred_guide = this->_file_info_strings->preferred_layout_guide();
    auto const file_info_actual_source = this->_file_info_strings->actual_layout_source();
    auto const &file_track_preferred_guide = this->_file_track_strings->preferred_layout_guide();
    auto const file_track_actual_source = this->_file_track_strings->actual_layout_source();
    auto const &marker_pool_preferred_guide = this->_marker_pool_strings->preferred_layout_guide();

    // status_strings

    ui::layout(safe_area_h_guide, status_preferred_guide->horizontal_range(),
               ui_layout_utils::constant(ui::range_insets::zero()))
        .sync()
        ->add_to(this->_pool);
    ui::layout(safe_area_guide->top(), status_preferred_guide->top(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);
    ui::layout(status_preferred_guide->top(), status_preferred_guide->bottom(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);

    // file_info_strings

    ui::layout(safe_area_h_guide, file_info_preferred_guide->horizontal_range(),
               ui_layout_utils::constant(ui::range_insets::zero()))
        .sync()
        ->add_to(this->_pool);
    ui::layout(status_actual_source->layout_vertical_range_source()->layout_min_value_source(),
               file_info_preferred_guide->top(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);
    ui::layout(file_info_preferred_guide->top(), file_info_preferred_guide->bottom(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);

    // file_track_strings

    ui::layout(safe_area_h_guide, file_track_preferred_guide->horizontal_range(),
               ui_layout_utils::constant(ui::range_insets::zero()))
        .sync()
        ->add_to(this->_pool);
    ui::layout(file_info_actual_source->layout_vertical_range_source()->layout_min_value_source(),
               file_track_preferred_guide->top(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);
    ui::layout(file_track_preferred_guide->top(), file_track_preferred_guide->bottom(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);

    // marker_pool_string

    ui::layout(safe_area_h_guide, marker_pool_preferred_guide->horizontal_range(),
               ui_layout_utils::constant(ui::range_insets::zero()))
        .sync()
        ->add_to(this->_pool);
    ui::layout(file_track_actual_source->layout_vertical_range_source()->layout_min_value_source(),
               marker_pool_preferred_guide->top(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);
    ui::layout(marker_pool_preferred_guide->top(), marker_pool_preferred_guide->bottom(),
               ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);

    // playing_line

    safe_area_guide
        ->observe([this](ui::region const &src_region) {
            ui::region const dst_region{.origin = {.x = src_region.center().x - 0.5f, .y = src_region.origin.y},
                                        .size = {.width = 1.0f, .height = src_region.size.height}};
            this->_playing_line->data()->set_rect_position(dst_region, 0);
        })
        .sync()
        ->add_to(this->_pool);

    // time

    ui::layout(this->_standard->view_look()->view_layout_guide()->top(), this->_time->top_layout_target(),
               ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<ui_editing_root> ui_editing_root::make_shared(std::shared_ptr<ui::standard> const &standard,
                                                              std::string const &project_id) {
    auto const texture = ui::texture::make_shared({.point_size = {1024, 1024}}, standard->view_look());
    auto const presenter = editing_root_presenter::make_shared(project_id);
    auto const action_controller = app::global()->project_pool()->project_for_id(project_id)->action_controller();
    auto const pinch_gesture_controller = pinch_gesture_controller::make_shared(project_id);
    auto const ui_track = ui_track::make_shared(standard, project_id);
    auto const ui_time = ui_time::make_shared(standard, texture, project_id);
    return std::shared_ptr<ui_editing_root>(new ui_editing_root{standard, texture, presenter, action_controller,
                                                                pinch_gesture_controller, ui_track, ui_time});
}
