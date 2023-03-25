//
//  ae_ui_grid.mm
//

#include "ae_ui_grid.hpp"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_types.h>
#include <audio_editor_core/ae_grid_presenter.hpp>
#include <audio_editor_core/ae_ui_atlas.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_grid_constants {
static std::size_t const reserving_interval = 100;
}

std::shared_ptr<ui_grid> ui_grid::make_shared(window_lifetime_id const &lifetime_id, ui::node *node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(lifetime_id);
    auto const presenter = grid_presenter::make_shared(lifetime_id);

    return std::make_shared<ui_grid>(presenter, resource_lifetime->standard, app_lifetime->color.get(),
                                     resource_lifetime->atlas.get(), node);
}

ui_grid::ui_grid(std::shared_ptr<grid_presenter> const &presenter, std::shared_ptr<ui::standard> const &standard,
                 ae::color *color, ui_atlas const *atlas, ui::node *node)
    : _presenter(presenter),
      _scale({1.0f, 1.0f}),
      _view_look(standard->view_look().get()),
      _color(color),
      _atlas(atlas),
      _node(node),
      _lines_node(ui::node::make_shared()) {
    this->_node->add_sub_node(this->_lines_node);

    auto const lines_mesh = ui::mesh::make_shared({.primitive_type = ui::primitive_type::line, .use_mesh_color = false},
                                                  nullptr, nullptr, atlas->texture());
    lines_mesh->set_use_mesh_color(true);
    this->_lines_node->set_mesh(lines_mesh);

    this->_set_line_count(0);

    presenter
        ->observe_contents([this](grid_content_pool_event const &event) {
            switch (event.type) {
                case module_content_pool_event_type::fetched:
                case module_content_pool_event_type::replaced:
                    this->_replace_data(event.elements);
                    break;
                case module_content_pool_event_type::updated:
                    if (this->_remaked_count < event.elements.size()) {
                        this->_replace_data(event.elements);
                    } else {
                        this->_update_data(event.elements.size(), event.inserted, event.replaced, event.erased);
                    }
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) { this->_presenter->update_contents(); })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            this->_lines_node->set_position({.x = 0.0f, .y = -region.size.height * 0.5f});
            this->_update_lines_scale();
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) {
            auto const &contents = this->_presenter->contents();
            this->_update_colors(contents);
        })
        .sync()
        ->add_to(this->_pool);

    atlas
        ->observe_white_filled_tex_coords([this](ui::uint_region const &tex_coords) {
            this->_vertex_data->write([&tex_coords](std::vector<ui::vertex2d_t> &vertices) {
                for (auto &vertex : vertices) {
                    vertex.tex_coord = to_float2(tex_coords.origin);
                }
            });
        })
        .sync()
        ->add_to(this->_pool);
}

void ui_grid::set_scale(ui::size const &scale) {
    this->_scale = scale;
    this->_update_lines_scale();
}

void ui_grid::_update_lines_scale() {
    auto const height = this->_view_look->view_layout_guide()->height()->value();
    this->_lines_node->set_scale({.width = this->_scale.width, .height = height});
}

void ui_grid::_replace_data(std::vector<std::optional<grid_content>> const &contents) {
    this->_set_line_count(contents.size());

    this->_vertex_data->write([&contents, this](std::vector<ui::vertex2d_t> &vertices) {
        auto *vertex_lines = (ae::vertex2d_line *)vertices.data();
        auto const &tex_coords = this->_atlas->white_filled_tex_coords();
        auto const color = this->_color->grid_line();

        auto each = make_fast_each(contents.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &content = contents.at(idx);
            auto &line = vertex_lines[idx];

            if (content.has_value()) {
                auto const &value = content.value();
                auto const x = value.x();
                line.set_position(ui::point{x, 0.0f}, ui::point{x, 1.0f});
                line.set_color(color);
                line.set_tex_coord(tex_coords.origin);
            } else {
                line.reset_position();
            }
        }
    });
}

void ui_grid::_update_data(std::size_t const count, std::vector<std::pair<std::size_t, grid_content>> const &inserted,
                           std::vector<std::pair<std::size_t, grid_content>> const &replaced,
                           std::vector<std::pair<std::size_t, grid_content>> const &erased) {
    this->_set_line_count(count);

    this->_vertex_data->write([&erased, &inserted, &replaced, this](std::vector<ui::vertex2d_t> &vertices) {
        auto *vertex_lines = (ae::vertex2d_line *)vertices.data();
        auto const &tex_coords = this->_atlas->white_filled_tex_coords();
        auto const color = this->_color->grid_line();

        for (auto const &pair : erased) {
            vertex_lines[pair.first].reset_position();
        }

        for (auto const &pair : inserted) {
            auto const &value = pair.second;
            auto const x = value.x();

            auto &line = vertex_lines[pair.first];
            line.set_position(ui::point{x, -0.5f}, ui::point{x, 1.0f});
            line.set_color(color);
            line.set_tex_coord(tex_coords.origin);
        }

        assert(replaced.empty());
    });
}

// こことは別に_update_dataで部分的に色を更新しているので、変更する際は注意
void ui_grid::_update_colors(std::vector<std::optional<grid_content>> const &contents) {
    this->_vertex_data->write([&contents, this](std::vector<ui::vertex2d_t> &vertices) {
        auto *vertex_lines = (ae::vertex2d_line *)vertices.data();

        auto const color = this->_color->grid_line();

        auto each = make_fast_each(contents.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &content = contents.at(idx);

            if (content.has_value()) {
                vertex_lines[idx].set_color(color);
            }
        }
    });
}

void ui_grid::_remake_data_if_needed(std::size_t const max_count) {
    if (max_count <= this->_remaked_count && (this->_vertex_data != nullptr || this->_index_data != nullptr)) {
        return;
    }

    auto const &lines_mesh = this->_lines_node->mesh();

    this->_vertex_data = nullptr;
    this->_index_data = nullptr;
    lines_mesh->set_vertex_data(nullptr);
    lines_mesh->set_index_data(nullptr);

    this->_vertex_data = ui::dynamic_mesh_vertex_data::make_shared(max_count * ae::vertex2d_line::vector_count);
    this->_index_data = ui::dynamic_mesh_index_data::make_shared(max_count * ae::index2d_line::vector_count);

    this->_index_data->write([&max_count](std::vector<ui::index2d_t> &indices) {
        auto *index_lines = (index2d_line *)indices.data();

        auto each = make_fast_each(max_count);
        while (yas_each_next(each)) {
            auto const &line_idx = yas_each_index(each);
            uint32_t const line_head_idx = static_cast<uint32_t>(line_idx * vertex2d_line::vector_count);
            index_lines[line_idx].set_all(line_head_idx);
        }
    });

    lines_mesh->set_vertex_data(this->_vertex_data);
    lines_mesh->set_index_data(this->_index_data);

    this->_remaked_count = max_count;
}

void ui_grid::_set_line_count(std::size_t const line_count) {
    this->_remake_data_if_needed(common_utils::reserving_count(line_count, ui_grid_constants::reserving_interval));

    if (this->_vertex_data) {
        this->_vertex_data->set_count(line_count * vertex2d_line::vector_count);
    }

    if (this->_index_data) {
        this->_index_data->set_count(line_count * index2d_line::vector_count);
    }
}
