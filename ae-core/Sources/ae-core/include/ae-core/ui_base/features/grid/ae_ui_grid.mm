//
//  ae_ui_grid.mm
//

#include "ae_ui_grid.hpp"
#include <ae-core/app/features/ae_color.h>
#include <ae-core/global/ae_hierarchy.h>
#include <ae-core/global/ae_ui_hierarchy.h>
#include <ae-core/global/utils/ae_common_utils.h>
#include <ae-core/ui_base/features/common/ae_ui_atlas.hpp>
#include <ae-core/ui_base/features/common/ae_ui_dynamic_mesh_container.hpp>
#include <ae-core/ui_base/features/grid/presenter/ae_grid_presenter.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_grid_constants {
static std::size_t const reserving_interval = 512;
}

namespace yas::ae::ui_grid_utils {
static std::unique_ptr<dynamic_mesh_content> make_element(std::size_t const idx, std::size_t const element_count) {
    auto vertex_data = ui::dynamic_mesh_vertex_data::make_shared(element_count * vertex2d_line::vector_count);
    auto index_data = ui::dynamic_mesh_index_data::make_shared(element_count * index2d_line::vector_count);
    auto mesh = ui::mesh::make_shared({.primitive_type = ui::primitive_type::line}, vertex_data, index_data, nullptr);
    mesh->set_use_mesh_color(true);

    return std::unique_ptr<dynamic_mesh_content>(new dynamic_mesh_content{
        .vertex_data = std::move(vertex_data), .index_data = std::move(index_data), .mesh = std::move(mesh)});
}
}  // namespace yas::ae::ui_grid_utils

std::shared_ptr<ui_grid> ui_grid::make_shared(project_lifetime_id const &lifetime_id, ui::node *node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(lifetime_id);
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
      _mesh_container(std::make_unique<dynamic_mesh_container<vertex2d_line, index2d_line>>(
          ui_grid_constants::reserving_interval, ui_grid_utils::make_element)) {
    this->_node->add_sub_node(this->_mesh_container->node);

    presenter
        ->observe_contents([this](grid_content_pool_event const &event) {
            switch (event.type) {
                case module_content_pool_event_type::fetched:
                case module_content_pool_event_type::replaced:
                    this->_replace_data();
                    break;
                case module_content_pool_event_type::updated:
                    this->_update_data(event.inserted, event.replaced, event.erased);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) {
            this->_presenter->update_contents();
            this->_mesh_container->reduce_if_needed();
        })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            this->_mesh_container->node->set_position({.x = 0.0f, .y = -region.size.height * 0.5f});
            this->_update_lines_scale();
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) { this->_update_colors(); })
        .sync()
        ->add_to(this->_pool);

    atlas
        ->observe_white_filled_tex_coords(
            [this](ui::uint_region const &tex_coords) { this->_update_tex_coords(tex_coords); })
        .sync()
        ->add_to(this->_pool);
}

void ui_grid::set_scale(ui::size const &scale) {
    this->_scale = scale;
    this->_update_lines_scale();
}

void ui_grid::_update_lines_scale() {
    auto const height = this->_view_look->view_layout_guide()->height()->value();
    this->_mesh_container->node->set_scale({.width = this->_scale.width, .height = height});
}

void ui_grid::_replace_data() {
    auto const &contents = this->_presenter->contents();

    this->_mesh_container->set_element_count(contents.size());

    this->_mesh_container->write_vertex_elements(
        [&contents, this](index_range const range, vertex2d_line *vertex_lines) {
            if (contents.size() <= range.index) {
                return;
            }

            auto const process_length = std::min(range.length, contents.size() - range.index);
            auto const &tex_coords = this->_atlas->white_filled_tex_coords();
            auto const color = this->_color->grid_line();

            auto each = make_fast_each(process_length);
            while (yas_each_next(each)) {
                auto const &vertex_idx = yas_each_index(each);
                auto const content_idx = range.index + vertex_idx;
                auto const &content = contents.at(content_idx);
                auto &line = vertex_lines[vertex_idx];

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

void ui_grid::_update_data(std::set<std::size_t> const &inserted, std::set<std::size_t> const &replaced,
                           std::map<std::size_t, grid_content> const &erased) {
    auto const &contents = this->_presenter->contents();

    this->_mesh_container->set_element_count(contents.size());

    this->_mesh_container->write_vertex_elements(
        [&contents, &erased, &inserted, &replaced, this](index_range const range, vertex2d_line *vertex_lines) {
            auto const &tex_coords = this->_atlas->white_filled_tex_coords();
            auto const color = this->_color->grid_line();

            for (auto const &pair : erased) {
                auto const &content_idx = pair.first;
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    vertex_lines[vertex_idx].reset_position();
                }
            }

            for (auto const &content_idx : inserted) {
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &value = contents.at(content_idx).value();
                    auto const x = value.x();

                    auto &line = vertex_lines[vertex_idx];
                    line.set_position(ui::point{x, -0.5f}, ui::point{x, 1.0f});
                    line.set_color(color);
                    line.set_tex_coord(tex_coords.origin);
                }
            }

            assert(replaced.empty());
        });
}

// こことは別に_update_dataで部分的に色を更新しているので、変更する際は注意
void ui_grid::_update_colors() {
    auto const &contents = this->_presenter->contents();

    this->_mesh_container->write_vertex_elements(
        [&contents, this](index_range const range, vertex2d_line *vertex_lines) {
            if (contents.size() <= range.index) {
                return;
            }

            auto const process_length = std::min(range.length, contents.size() - range.index);
            auto const color = this->_color->grid_line();

            auto each = make_fast_each(process_length);
            while (yas_each_next(each)) {
                auto const &vertex_idx = yas_each_index(each);
                auto const content_idx = range.index + vertex_idx;
                auto const &content = contents.at(content_idx);

                if (content.has_value()) {
                    vertex_lines[vertex_idx].set_color(color);
                }
            }
        });
}

void ui_grid::_update_tex_coords(ui::uint_region const &tex_coords) {
    this->_mesh_container->write_vertex_elements([&tex_coords](index_range const range, vertex2d_line *vertex_lines) {
        auto each = make_fast_each(range.length);
        while (yas_each_next(each)) {
            auto const &line_idx = yas_each_index(each);
            vertex_lines[line_idx].set_tex_coord(tex_coords.origin);
        }
    });
}
