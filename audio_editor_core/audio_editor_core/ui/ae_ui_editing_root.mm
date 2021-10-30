//
//  ae_ui_root.cpp
//

#include "ae_ui_editing_root.h"
#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_editing_root_presenter.h>
#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_keyboard.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_ui_editing_root_utils.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_track.h>

using namespace yas;
using namespace yas::ae;

ui_editing_root::ui_editing_root(std::shared_ptr<ui::standard> const &standard,
                                 std::shared_ptr<editing_root_presenter> const &presenter,
                                 std::shared_ptr<action_controller> const &action_controller,
                                 std::shared_ptr<pinch_gesture_controller> const &pinch_gesture_controller,
                                 std::shared_ptr<ui_track> const &track)
    : _presenter(presenter),
      _action_controller(action_controller),
      _pinch_gesture_controller(pinch_gesture_controller),
      _standard(standard),
      _texture(ui::texture::make_shared({.point_size = {1024, 1024}}, standard->view_look())),
      _keyboard(ae::keyboard::make_shared(standard->event_manager())),
      _font_atlas(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold",
           .font_size = 14.0f,
           .words = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:[]"},
          _texture)),
      _play_button(ui_button::make_shared(this->_font_atlas, standard)),
      _split_button(ui_button::make_shared(this->_font_atlas, standard)),
      _drop_head_and_offset_button(ui_button::make_shared(this->_font_atlas, standard)),
      _drop_tail_and_offset_button(ui_button::make_shared(this->_font_atlas, standard)),
      _erase_and_offset_button(ui_button::make_shared(this->_font_atlas, standard)),
      _zero_button(ui_button::make_shared(this->_font_atlas, standard)),
      _jump_previous_button(ui_button::make_shared(this->_font_atlas, standard)),
      _jump_next_button(ui_button::make_shared(this->_font_atlas, standard)),
      _insert_marker_button(ui_button::make_shared(this->_font_atlas, standard)),
      _buttons({this->_play_button, this->_split_button, this->_drop_head_and_offset_button,
                this->_drop_tail_and_offset_button, this->_erase_and_offset_button, this->_zero_button,
                this->_jump_previous_button, this->_jump_next_button, this->_insert_marker_button}),
      _button_collection_layout(ui::collection_layout::make_shared({.preferred_cell_count = this->_buttons.size(),
                                                                    .row_spacing = 1.0f,
                                                                    .col_spacing = 1.0f,
                                                                    .default_cell_size = {60.0f, 44.0f},
                                                                    .row_order = ui::layout_order::descending})),
      _status_strings(ui::strings::make_shared(
          {.text = "", .alignment = ui::layout_alignment::min, .max_word_count = 128}, this->_font_atlas)),
      _file_info_strings(ui::strings::make_shared(
          {.text = "", .alignment = ui::layout_alignment::min, .max_word_count = 128}, this->_font_atlas)),
      _player_strings(ui::strings::make_shared(
          {.text = "", .alignment = ui::layout_alignment::min, .max_word_count = 128}, this->_font_atlas)),
      _file_track_strings(ui::strings::make_shared(
          {.text = "", .alignment = ui::layout_alignment::min, .max_word_count = 1024}, this->_font_atlas)),
      _marker_pool_strings(ui::strings::make_shared(
          {.text = "", .alignment = ui::layout_alignment::min, .max_word_count = 1024}, this->_font_atlas)),
      _track(track),
      _playing_line(ui::rect_plane::make_shared(1)) {
    standard->view_look()->background()->set_color({.v = 0.2f});

    this->_split_button->set_text("split");
    this->_drop_head_and_offset_button->set_text("drop\nhead");
    this->_drop_tail_and_offset_button->set_text("drop\ntail");
    this->_erase_and_offset_button->set_text("erase");
    this->_zero_button->set_text("zero");
    this->_jump_previous_button->set_text("jump\nprev");
    this->_jump_next_button->set_text("jump\nnext");
    this->_insert_marker_button->set_text("insert\nmarker");

    this->_setup_node_hierarchie();
    this->_setup_observing();
    this->_setup_layout();
}

void ui_editing_root::_setup_node_hierarchie() {
    auto const &root_node = this->_standard->root_node();

    root_node->add_sub_node(this->_track->node());

    root_node->add_sub_node(this->_play_button->node());
    root_node->add_sub_node(this->_split_button->node());
    root_node->add_sub_node(this->_drop_head_and_offset_button->node());
    root_node->add_sub_node(this->_drop_tail_and_offset_button->node());
    root_node->add_sub_node(this->_erase_and_offset_button->node());
    root_node->add_sub_node(this->_zero_button->node());
    root_node->add_sub_node(this->_jump_previous_button->node());
    root_node->add_sub_node(this->_jump_next_button->node());
    root_node->add_sub_node(this->_insert_marker_button->node());

    root_node->add_sub_node(this->_status_strings->rect_plane()->node());
    root_node->add_sub_node(this->_file_info_strings->rect_plane()->node());
    root_node->add_sub_node(this->_player_strings->rect_plane()->node());
    root_node->add_sub_node(this->_file_track_strings->rect_plane()->node());
    root_node->add_sub_node(this->_marker_pool_strings->rect_plane()->node());

    root_node->add_sub_node(this->_playing_line->node());
}

void ui_editing_root::_setup_observing() {
    auto const &presenter = this->_presenter;

    presenter->observe_state_text([this](std::string const &string) { this->_status_strings->set_text(string); })
        .sync()
        ->add_to(this->_pool);

    presenter->observe_file_info_text([this](std::string const &string) { this->_file_info_strings->set_text(string); })
        .sync()
        ->add_to(this->_pool);

    presenter->observe_play_button_text([this](std::string const &string) { this->_play_button->set_text(string); })
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
            this->_player_strings->set_text(this->_presenter->player_text());
            this->_update_buttons_enabled();

            this->_playing_line->node()->set_color(
                ui_editing_root_utils::to_playing_line_color(this->_presenter->playing_line_state()));
        })
        .end()
        ->add_to(this->_pool);

    this->_play_button->observe_tapped([this] { this->_action_controller->handle_action(action::toggle_play); })
        .end()
        ->add_to(this->_pool);
    this->_split_button->observe_tapped([this] { this->_action_controller->handle_action(action::split); })
        .end()
        ->add_to(this->_pool);
    this->_drop_head_and_offset_button
        ->observe_tapped([this] { this->_action_controller->handle_action(action::drop_head_and_offset); })
        .end()
        ->add_to(this->_pool);
    this->_drop_tail_and_offset_button
        ->observe_tapped([this] { this->_action_controller->handle_action(action::drop_tail_and_offset); })
        .end()
        ->add_to(this->_pool);
    this->_erase_and_offset_button
        ->observe_tapped([this] { this->_action_controller->handle_action(action::erase_and_offset); })
        .end()
        ->add_to(this->_pool);
    this->_zero_button->observe_tapped([this] { this->_action_controller->handle_action(action::return_to_zero); })
        .end()
        ->add_to(this->_pool);
    this->_jump_previous_button
        ->observe_tapped([this] { this->_action_controller->handle_action(action::jump_previous); })
        .end()
        ->add_to(this->_pool);
    this->_jump_next_button->observe_tapped([this] { this->_action_controller->handle_action(action::jump_next); })
        .end()
        ->add_to(this->_pool);
    this->_insert_marker_button
        ->observe_tapped([this] { this->_action_controller->handle_action(action::insert_marker); })
        .end()
        ->add_to(this->_pool);

    this->_keyboard
        ->observe([this](ae::key const &key) {
            this->_action_controller->handle_action(ui_editing_root_utils::to_action(key));
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
    auto const &button_collection_guide = this->_button_collection_layout->preferred_layout_guide();
    auto const &button_collection_actual_source = this->_button_collection_layout->actual_frame_layout_source();
    auto const &status_preferred_guide = this->_status_strings->preferred_layout_guide();
    auto const status_actual_source = this->_status_strings->actual_layout_source();
    auto const &file_info_preferred_guide = this->_file_info_strings->preferred_layout_guide();
    auto const file_info_actual_source = this->_file_info_strings->actual_layout_source();
    auto const &player_preferred_guide = this->_player_strings->preferred_layout_guide();
    auto const player_actual_source = this->_player_strings->actual_layout_source();
    auto const &file_track_preferred_guide = this->_file_track_strings->preferred_layout_guide();
    auto const file_track_actual_source = this->_file_track_strings->actual_layout_source();
    auto const &marker_pool_preferred_guide = this->_marker_pool_strings->preferred_layout_guide();

    // button_collection

    ui::layout(safe_area_guide, button_collection_guide, ui_layout_utils::constant(ui::region_insets::zero()))
        .sync()
        ->add_to(this->_pool);

    this->_button_collection_layout
        ->observe_actual_cell_regions([buttons = this->_buttons](auto const &regions) {
            auto each = make_fast_each(buttons.size());
            while (yas_each_next(each)) {
                auto const &idx = yas_each_index(each);
                auto const &button = buttons.at(idx);

                if (idx < regions.size()) {
                    button->node()->set_is_enabled(true);
                    button->layout_guide()->set_region(regions.at(idx));
                } else {
                    button->node()->set_is_enabled(false);
                }
            }
        })
        .sync()
        ->add_to(this->_pool);

    // status_strings

    ui::layout(safe_area_h_guide, status_preferred_guide->horizontal_range(),
               ui_layout_utils::constant(ui::range_insets::zero()))
        .sync()
        ->add_to(this->_pool);
    ui::layout(button_collection_actual_source->layout_vertical_range_source()->layout_min_value_source(),
               status_preferred_guide->top(), ui_layout_utils::constant(0.0f))
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

    // player_strings

    ui::layout(safe_area_h_guide, player_preferred_guide->horizontal_range(),
               ui_layout_utils::constant(ui::range_insets::zero()))
        .sync()
        ->add_to(this->_pool);
    ui::layout(file_info_actual_source->layout_vertical_range_source()->layout_min_value_source(),
               player_preferred_guide->top(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);
    ui::layout(player_preferred_guide->top(), player_preferred_guide->bottom(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);

    // file_track_strings

    ui::layout(safe_area_h_guide, file_track_preferred_guide->horizontal_range(),
               ui_layout_utils::constant(ui::range_insets::zero()))
        .sync()
        ->add_to(this->_pool);
    ui::layout(player_actual_source->layout_vertical_range_source()->layout_min_value_source(),
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
}

void ui_editing_root::_update_buttons_enabled() {
    auto const &presenter = this->_presenter;

    this->_play_button->set_enabled(presenter->is_play_button_enabled());
    this->_split_button->set_enabled(presenter->is_split_button_enabled());
    this->_drop_head_and_offset_button->set_enabled(presenter->is_drop_head_and_offset_button_enabled());
    this->_drop_tail_and_offset_button->set_enabled(presenter->is_drop_tail_and_offset_button_enabled());
    this->_jump_previous_button->set_enabled(presenter->is_jump_previous_button_enabled());
    this->_jump_next_button->set_enabled(presenter->is_jump_next_button_enabled());
    this->_erase_and_offset_button->set_enabled(presenter->is_erase_and_offset_button_enabled());
    this->_insert_marker_button->set_enabled(presenter->is_insert_marker_button_enabled());
    this->_zero_button->set_enabled(presenter->is_zero_button_enabled());
}

std::shared_ptr<ui_editing_root> ui_editing_root::make_shared(std::shared_ptr<ui::standard> const &standard,
                                                              std::string const &project_id) {
    auto const presenter = editing_root_presenter::make_shared(project_id);
    auto const action_controller = action_controller::make_shared(project_id);
    auto const pinch_gesture_controller = pinch_gesture_controller::make_shared(project_id);
    auto const ui_track = ui_track::make_shared(standard, project_id);
    return std::shared_ptr<ui_editing_root>(
        new ui_editing_root{standard, presenter, action_controller, pinch_gesture_controller, ui_track});
}
