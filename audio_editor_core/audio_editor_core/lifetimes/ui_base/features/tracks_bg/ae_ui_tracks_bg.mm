//
//  ae_ui_tracks_bg.cpp
//

#include "ae_ui_tracks_bg.hpp"

#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_tracks_presenter.hpp>
#include <audio_editor_core/ae_ui_atlas.hpp>
#include <audio_editor_core/ae_ui_mesh_utils.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_tracks_bg_constants {
static std::size_t constexpr reserving_interval = 64;
static float constexpr square_height = 0.95f;
}

std::shared_ptr<ui_tracks_bg> ui_tracks_bg::make_shared(project_lifetime_id const &project_lifetime_id,
                                                        ui::node *node) {
    auto const presenter = tracks_presenter::make_shared(project_lifetime_id);
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);
    return std::make_shared<ui_tracks_bg>(project_lifetime_id, node, presenter, resource_lifetime->standard,
                                          app_lifetime->color.get(), resource_lifetime->atlas.get());
}

ui_tracks_bg::ui_tracks_bg(project_lifetime_id const &project_lifetime_id, ui::node *node,
                           std::shared_ptr<tracks_presenter> const &presenter,
                           std::shared_ptr<ui::standard> const &standard, ae::color *color, ui_atlas const *atlas)
    : _project_lifetime_id(project_lifetime_id),
      _node(node),
      _presenter(presenter),
      _color(color),
      _atlas(atlas),
      _left_guide(standard->view_look()->view_layout_guide()->left()),
      _width_guide(standard->view_look()->view_layout_guide()->width()),
      _vertex_datas(std::make_unique<std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>>>()),
      _fill_mesh_container(
          ui_mesh_utils::make_fill_container(this->_vertex_datas.get(), ui_tracks_bg_constants::reserving_interval)) {
    node->add_sub_node(this->_fill_mesh_container->node);

    presenter
        ->observe_contents([this](track_content_pool_event const &event) {
            switch (event.type) {
                case track_content_pool_event_type::fetched:
                case track_content_pool_event_type::replaced:
                    this->_replace_data();
                    break;
                case track_content_pool_event_type::updated:
                    this->_update_data(event.inserted, event.replaced, event.erased);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) { this->_presenter->update_if_needed(); })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) { this->_update_colors(); })
        .sync()
        ->add_to(this->_pool);

    atlas
        ->observe_white_filled_tex_coords(
            [vertex_datas = this->_vertex_datas.get()](ui::uint_region const &tex_coords) {
                for (auto const &vertex_data : *vertex_datas) {
                    vertex_data->write([&tex_coords](std::vector<ui::vertex2d_t> &vertices) {
                        for (auto &vertex : vertices) {
                            vertex.tex_coord = to_float2(tex_coords.origin);
                        }
                    });
                }
            })
        .sync()
        ->add_to(this->_pool);

    this->_fill_mesh_container->node->attach_x_layout_guide(*this->_left_guide);

    this->_width_guide->observe([this](float const &width) { this->_fill_mesh_container->node->set_width(width); })
        .sync()
        ->add_to(this->_pool);
}

void ui_tracks_bg::set_scale(ui::size const &scale) {
    this->_scale = scale;

    this->_fill_mesh_container->node->set_height(scale.height);
}

void ui_tracks_bg::_replace_data() {
    auto const &contents = this->_presenter->contents();

    this->_set_rect_count(contents.size());

    this->_fill_mesh_container->write_vertex_elements(
        [this, &contents](index_range const range, vertex2d_rect *vertex_rects) {
            if (contents.size() <= range.index) {
                return;
            }

            auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

            auto const process_length = std::min(range.length, contents.size() - range.index);
            auto each = make_fast_each(process_length);
            while (yas_each_next(each)) {
                auto const &vertex_idx = yas_each_index(each);
                auto const content_idx = range.index + vertex_idx;

                auto const &content = contents.at(content_idx);
                auto &rect = vertex_rects[vertex_idx];

                if (content.has_value()) {
                    auto const &value = content.value();
                    ui::region const region{
                        .origin = {.x = 0.0f, .y = value.track() - (ui_tracks_bg_constants::square_height * 0.5f)},
                        .size = {.width = 1.0f, .height = ui_tracks_bg_constants::square_height}};
                    rect.set_position(region);
                    rect.set_tex_coord(filled_tex_coords);
                } else {
                    rect.set_position(ui::region::zero());
                }
            }
        });

    this->_update_colors();
}

// こことは別に_update_dataで部分的に色を更新しているので、変更する際は注意
void ui_tracks_bg::_update_colors() {
    auto const &contents = this->_presenter->contents();

    this->_fill_mesh_container->write_vertex_elements([this, &contents](index_range const range,
                                                                        vertex2d_rect *vertex_rects) {
        if (contents.size() <= range.index) {
            return;
        }

        auto const normal_square_color = this->_color->track_bg().v;
        auto const selected_square_color = this->_color->selected_track_bg().v;
        auto const process_length = std::min(range.length, contents.size() - range.index);

        auto each = make_fast_each(process_length);
        while (yas_each_next(each)) {
            auto const &vertex_idx = yas_each_index(each);
            auto const content_idx = range.index + vertex_idx;
            auto const &content = contents.at(content_idx);

            if (content.has_value()) {
                auto const &square_color = content.value().is_selected ? selected_square_color : normal_square_color;
                vertex_rects[vertex_idx].set_color(square_color);
            }
        }
    });
}

void ui_tracks_bg::_update_data(std::set<std::size_t> const &inserted, std::set<std::size_t> const &replaced,
                                std::map<std::size_t, track_content> const &erased) {
    auto const &contents = this->_presenter->contents();

    this->_set_rect_count(contents.size());

    this->_fill_mesh_container->write_vertex_elements(
        [&contents, &erased, &inserted, &replaced, this](index_range const range, vertex2d_rect *vertex_rects) {
            for (auto const &pair : erased) {
                auto const &content_idx = pair.first;
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    vertex_rects[vertex_idx].set_position(ui::region::zero());
                }
            }

            auto const &color = this->_color;
            auto const normal_square_color = color->track_bg().v;
            auto const selected_square_color = color->selected_track_bg().v;
            auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

            for (auto const &content_idx : inserted) {
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &value = contents.at(content_idx).value();
                    ui::region const region{
                        .origin = {.x = 0.0f, .y = value.track() - (ui_tracks_bg_constants::square_height * 0.5f)},
                        .size = {.width = 1.0f, .height = ui_tracks_bg_constants::square_height}};

                    auto &rect = vertex_rects[vertex_idx];
                    rect.set_position(region);
                    rect.set_tex_coord(filled_tex_coords);

                    auto const &square_color = value.is_selected ? selected_square_color : normal_square_color;
                    rect.set_color(square_color);
                }
            }

            for (auto const &content_idx : replaced) {
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &value = contents.at(content_idx).value();
                    auto const &square_color = value.is_selected ? selected_square_color : normal_square_color;
                    vertex_rects[vertex_idx].set_color(square_color);
                }
            }
        });
}

void ui_tracks_bg::_set_rect_count(std::size_t const rect_count) {
    this->_fill_mesh_container->set_element_count(rect_count);
}
