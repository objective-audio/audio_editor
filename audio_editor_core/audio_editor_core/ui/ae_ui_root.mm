//
//  ae_ui_root.cpp
//

#include "ae_ui_root.h"
#include <audio_editor_core/ae_project_view_presenter.h>
#include <audio_editor_core/ae_ui_layout_utils.h>

using namespace yas;
using namespace yas::ae;

ui_root::ui_root(std::shared_ptr<ui::renderer> const &renderer,
                 std::shared_ptr<project_view_presenter> const &presenter)
    : _presenter(presenter),
      _renderer(renderer),
      _font_atlas(
          ui::font_atlas::make_shared({.font_name = "TrebuchetMS-Bold",
                                       .font_size = 14.0f,
                                       .words = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-"})),
      _button_collection_layout(ui::collection_layout::make_shared({.preferred_cell_count = 4,
                                                                    .row_spacing = 1.0f,
                                                                    .col_spacing = 1.0f,
                                                                    .default_cell_size = {60.0f, 44.0f},
                                                                    .row_order = ui::layout_order::descending})),
      _play_button(ui_button::make_shared(this->_font_atlas)),
      _split_button(ui_button::make_shared(this->_font_atlas)),
      _drop_head_and_offset_button(ui_button::make_shared(this->_font_atlas)),
      _drop_tail_and_offset_button(ui_button::make_shared(this->_font_atlas)),
      _buttons({this->_play_button, this->_split_button, this->_drop_head_and_offset_button,
                this->_drop_tail_and_offset_button}),
      _status_strings(ui::strings::make_shared({.text = "",
                                                .alignment = ui::layout_alignment::min,
                                                .font_atlas = this->_font_atlas,
                                                .max_word_count = 128})),
      _file_info_strings(ui::strings::make_shared({.text = "",
                                                   .alignment = ui::layout_alignment::min,
                                                   .font_atlas = this->_font_atlas,
                                                   .max_word_count = 128})),
      _player_strings(ui::strings::make_shared({.text = "",
                                                .alignment = ui::layout_alignment::min,
                                                .font_atlas = this->_font_atlas,
                                                .max_word_count = 128})),
      _file_track_strings(ui::strings::make_shared({.text = "",
                                                    .alignment = ui::layout_alignment::min,
                                                    .font_atlas = this->_font_atlas,
                                                    .max_word_count = 1024})) {
    auto const texture = ui::texture::make_shared({.point_size = {1024, 1024}});
    texture->sync_scale_from_renderer(this->_renderer);

    this->_font_atlas->set_texture(texture);

    this->_renderer->background()->set_color({.v = 0.2f});

    this->_split_button->set_text("split");
    this->_drop_head_and_offset_button->set_text("drop\nhead");
    this->_drop_tail_and_offset_button->set_text("drop\ntail");

    this->_setup_node_hierarchie();
    this->_setup_observing();
    this->_setup_layout();
}

void ui_root::_setup_node_hierarchie() {
    auto const &root_node = this->_renderer->root_node();

    root_node->add_sub_node(this->_play_button->node());
    root_node->add_sub_node(this->_split_button->node());
    root_node->add_sub_node(this->_drop_head_and_offset_button->node());
    root_node->add_sub_node(this->_drop_tail_and_offset_button->node());
    root_node->add_sub_node(this->_status_strings->rect_plane()->node());
    root_node->add_sub_node(this->_file_info_strings->rect_plane()->node());
    root_node->add_sub_node(this->_player_strings->rect_plane()->node());
    root_node->add_sub_node(this->_file_track_strings->rect_plane()->node());
}

void ui_root::_setup_observing() {
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

    this->_renderer
        ->observe_will_render(
            [this](auto const &) { this->_player_strings->set_text(this->_presenter->player_text()); })
        .end()
        ->add_to(this->_pool);

    this->_play_button->observe_tapped([this] { this->_presenter->play_button_clicked(); }).end()->add_to(this->_pool);
    this->_split_button->observe_tapped([this] { this->_presenter->split_button_clicked(); })
        .end()
        ->add_to(this->_pool);
    this->_drop_head_and_offset_button->observe_tapped([this] { this->_presenter->drop_head_button_clicked(); })
        .end()
        ->add_to(this->_pool);
    this->_drop_tail_and_offset_button->observe_tapped([this] { this->_presenter->drop_tail_button_clicked(); })
        .end()
        ->add_to(this->_pool);
}

void ui_root::_setup_layout() {
    auto const &safe_area_guide = this->_renderer->safe_area_layout_guide();
    auto const &safe_area_h_guide = this->_renderer->safe_area_layout_guide()->horizontal_range();
    auto const &button_collection_guide = this->_button_collection_layout->preferred_layout_guide();
    auto const &button_collection_actual_source = this->_button_collection_layout->actual_frame_layout_source();
    auto const &status_preferred_guide = this->_status_strings->preferred_layout_guide();
    auto const status_actual_source = this->_status_strings->actual_layout_source();
    auto const &file_info_preferred_guide = this->_file_info_strings->preferred_layout_guide();
    auto const file_info_actual_source = this->_file_info_strings->actual_layout_source();
    auto const &player_preferred_guide = this->_player_strings->preferred_layout_guide();
    auto const player_actual_source = this->_player_strings->actual_layout_source();
    auto const &file_track_preferred_guide = this->_file_track_strings->preferred_layout_guide();

    // button_collection

    ui::layout(safe_area_guide, button_collection_guide, ui_layout_utils::constant(ui::region_insets::zero()))
        .sync()
        ->add_to(this->_pool);

    this->_button_collection_layout
        ->observe_actual_cell_layout_guides(
            [pool = std::make_shared<observing::canceller_pool>(), buttons = this->_buttons](auto const &cell_guides) {
                pool->cancel();

                auto each = make_fast_each(buttons.size());
                while (yas_each_next(each)) {
                    auto const &idx = yas_each_index(each);
                    auto const &button = buttons.at(idx);

                    if (idx < cell_guides.size()) {
                        button->node()->set_is_enabled(true);
                        ui::layout(cell_guides.at(idx), button->layout_guide(),
                                   ui_layout_utils::constant(ui::region_insets::zero()))
                            .sync()
                            ->add_to(*pool);
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
}

std::shared_ptr<ui::renderer> const &ui_root::renderer() const {
    return this->_renderer;
}

std::shared_ptr<ui_root> ui_root::make_shared(std::shared_ptr<ui::renderer> const &renderer,
                                              std::shared_ptr<project_view_presenter> const &presenter) {
    return std::shared_ptr<ui_root>(new ui_root{renderer, presenter});
}
