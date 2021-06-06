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
      _play_button(ui::button::make_shared({.size = {44, 44}})),
      _font_atlas(
          ui::font_atlas::make_shared({.font_name = "TrebuchetMS-Bold",
                                       .font_size = 14.0f,
                                       .words = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-"})),
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
                                                .max_word_count = 128})) {
    auto const texture = ui::texture::make_shared({.point_size = {1024, 1024}});
    texture->sync_scale_from_renderer(this->_renderer);

    this->_font_atlas->set_texture(texture);

    this->_renderer->background()->set_color({.v = 0.1f});

    this->_play_button->rect_plane()->node()->mesh()->set_use_mesh_color(true);
    this->_play_button->rect_plane()->data()->set_rect_color(to_float4(ui::orange_color(), 1.0f),
                                                             to_rect_index(0, false));
    this->_play_button->rect_plane()->data()->set_rect_color(to_float4(ui::cyan_color(), 1.0f), to_rect_index(0, true));

    auto const &root_node = this->_renderer->root_node();

    root_node->add_sub_node(this->_play_button->rect_plane()->node());
    root_node->add_sub_node(this->_status_strings->rect_plane()->node());
    root_node->add_sub_node(this->_file_info_strings->rect_plane()->node());
    root_node->add_sub_node(this->_player_strings->rect_plane()->node());

    presenter->observe_state_string([this](std::string const &string) { this->_status_strings->set_text(string); })
        .sync()
        ->add_to(this->_pool);

    presenter
        ->observe_file_info_string([this](std::string const &string) { this->_file_info_strings->set_text(string); })
        .sync()
        ->add_to(this->_pool);

    this->_renderer
        ->observe_will_render(
            [this](auto const &) { this->_player_strings->set_text(this->_presenter->player_string()); })
        .end()
        ->add_to(this->_pool);

    this->_play_button
        ->observe([this](auto const &context) {
            if (context.method == ui::button::method::ended) {
                this->_presenter->play_button_clicked();
            }
        })
        .end()
        ->add_to(this->_pool);

    // layout

    auto const &safe_area_guide = this->_renderer->safe_area_layout_region_guide();
    auto const &safe_area_h_guide = this->_renderer->safe_area_layout_region_guide()->horizontal_range();
    auto const &play_button_frame_guide = this->_play_button->layout_region_guide();
    auto const &status_frame_guide = this->_status_strings->frame_layout_region_guide();
    auto const &file_info_frame_guide = this->_file_info_strings->frame_layout_region_guide();
    auto const &player_frame_guide = this->_player_strings->frame_layout_region_guide();
    auto const status_actual_guide = ui::layout_region_guide::make_shared();
    auto const file_info_actual_guide = ui::layout_region_guide::make_shared();

    ui::layout(safe_area_guide, play_button_frame_guide,
               [](ui::region const &source) {
                   return ui::region{.origin = {source.left(), source.top() - 44.0f}, .size = {44.0f, 44.0f}};
               })
        .sync()
        ->add_to(this->_pool);

    ui::layout(safe_area_h_guide, status_frame_guide->horizontal_range(),
               ui_layout_utils::constant(ui::range_insets::zero()))
        .sync()
        ->add_to(this->_pool);
    ui::layout(play_button_frame_guide->bottom(), status_frame_guide->top(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);
    ui::layout(play_button_frame_guide->bottom(), status_frame_guide->bottom(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);

    this->_status_strings
        ->observe_actual_frame([status_actual_guide](auto const &region) {
            if (region.has_value()) {
                status_actual_guide->set_region(region.value());
            }
        })
        .sync()
        ->add_to(this->_pool);

    ui::layout(safe_area_h_guide, file_info_frame_guide->horizontal_range(),
               ui_layout_utils::constant(ui::range_insets::zero()))
        .sync()
        ->add_to(this->_pool);
    ui::layout(status_actual_guide->bottom(), file_info_frame_guide->top(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);
    ui::layout(status_actual_guide->bottom(), file_info_frame_guide->bottom(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);

    this->_file_info_strings
        ->observe_actual_frame([file_info_actual_guide](auto const &region) {
            if (region.has_value()) {
                file_info_actual_guide->set_region(region.value());
            }
        })
        .sync()
        ->add_to(this->_pool);

    ui::layout(safe_area_h_guide, player_frame_guide->horizontal_range(),
               ui_layout_utils::constant(ui::range_insets::zero()))
        .sync()
        ->add_to(this->_pool);
    ui::layout(file_info_actual_guide->bottom(), player_frame_guide->top(), ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);
    ui::layout(file_info_actual_guide->bottom(), player_frame_guide->bottom(), ui_layout_utils::constant(0.0f))
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
