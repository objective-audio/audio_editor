//
//  ae_ui_editing_root.cpp
//

#include "ae_ui_editing_root.h"
#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_editing_root_presenter.h>
#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_keyboard.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_ui_editing_root_utils.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_root_level.h>
#include <audio_editor_core/ae_ui_time.h>
#include <audio_editor_core/ae_ui_track.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_editing_root> ui_editing_root::make_shared(ui_project_id const &project_id) {
    auto const &app_level = app_level::global();
    auto const &ui_root_level = ui_hierarchy::root_level_for_view_id(project_id.view_id);

    auto const presenter = editing_root_presenter::make_shared(project_id.identifier);

    auto const &project_level = hierarchy::project_level_for_id(project_id.identifier);
    auto const ui_track = ui_track::make_shared(project_id);
    auto const ui_time = ui_time::make_shared(project_id);

    return std::shared_ptr<ui_editing_root>(
        new ui_editing_root{ui_root_level->standard, ui_root_level->font_atlas_14, app_level->color, presenter,
                            project_level->action_controller, ui_root_level->pinch_gesture_controller,
                            ui_root_level->keyboard, ui_track, ui_time});
}

ui_editing_root::ui_editing_root(std::shared_ptr<ui::standard> const &standard,
                                 std::shared_ptr<ui::font_atlas> const &font_atlas,
                                 std::shared_ptr<ae::color> const &color,
                                 std::shared_ptr<editing_root_presenter> const &presenter,
                                 std::shared_ptr<action_controller> const &action_controller,
                                 std::shared_ptr<pinch_gesture_controller> const &pinch_gesture_controller,
                                 std::shared_ptr<ae::keyboard> const &keyboard, std::shared_ptr<ui_track> const &track,
                                 std::shared_ptr<ui_time> const &time)
    : node(ui::node::make_shared()),
      _presenter(presenter),
      _action_controller(action_controller),
      _pinch_gesture_controller(pinch_gesture_controller),
      _standard(standard),
      _color(color),
      _font_atlas(font_atlas),
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
    this->_file_info_strings->set_text(presenter->file_info_text());

    this->_setup_node_hierarchie();
    this->_setup_observing(keyboard);
    this->_setup_layout();
}

bool ui_editing_root::responds_to_action(action const action) {
    return this->_presenter->responds_to_action(action);
}

void ui_editing_root::_setup_node_hierarchie() {
    this->node->add_sub_node(this->_track->node);

    this->node->add_sub_node(this->_status_strings->rect_plane()->node());
    this->node->add_sub_node(this->_file_info_strings->rect_plane()->node());
    this->node->add_sub_node(this->_file_track_strings->rect_plane()->node());
    this->node->add_sub_node(this->_marker_pool_strings->rect_plane()->node());

    this->node->add_sub_node(this->_playing_line->node());
    this->node->add_sub_node(this->_time->node());
}

void ui_editing_root::_setup_observing(std::shared_ptr<ae::keyboard> const &keyboard) {
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
            this->_playing_line->node()->set_color(
                ui_editing_root_utils::to_playing_line_color(this->_presenter->playing_line_state(), this->_color));
        })
        .end()
        ->add_to(this->_pool);

    keyboard
        ->observe_key([this](ae::key const &key) {
            if (auto const controller = this->_action_controller.lock()) {
                controller->handle_key(key);
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

    this->_standard->event_manager()
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            if (event->type() == ui::event_type::pinch) {
                auto const &pinch_event = event->get<ui::pinch>();
                gesture_state const state = to_gesture_state(event->phase());

                if (auto const controller = this->_pinch_gesture_controller.lock()) {
                    controller->handle_gesture(
                        pinch_gesture{.state = state, .magnification = pinch_event.magnification()});
                }
            }
        })
        .end()
        ->add_to(this->_pool);

    this->_standard->view_look()
        ->observe_appearance([this](auto const &) {
            auto const &color = this->_color;
            this->_standard->view_look()->background()->set_color(color->background());
            this->_status_strings->rect_plane()->node()->set_color(color->debug_text());
            this->_file_info_strings->rect_plane()->node()->set_color(color->debug_text());
            this->_file_track_strings->rect_plane()->node()->set_color(color->debug_text());
            this->_marker_pool_strings->rect_plane()->node()->set_color(color->debug_text());
        })
        .sync()
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
