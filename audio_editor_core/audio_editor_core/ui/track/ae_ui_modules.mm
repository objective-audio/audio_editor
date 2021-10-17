//
//  ae_ui_modules.mm
//

#include "ae_ui_modules.h"
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_module_location.h>
#include <audio_editor_core/ae_modules_presenter.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_modules_constants {
static std::size_t const reserving_interval = 100;
}

void ui_modules::line_index2d_rect::set_all(ui::index2d_t const first) {
    this->v[0] = this->v[7] = first;
    this->v[1] = this->v[2] = first + 1;
    this->v[3] = this->v[4] = first + 3;
    this->v[5] = this->v[6] = first + 2;
}

ui_modules::ui_modules(std::shared_ptr<modules_presenter> const &presenter)
    : _presenter(presenter),
      _node(ui::node::make_shared()),
      _triangle_node(ui::node::make_shared()),
      _line_node(ui::node::make_shared()),
      _triangle_mesh(ui::mesh::make_shared({.use_mesh_color = true}, nullptr, nullptr, nullptr)),
      _line_mesh(ui::mesh::make_shared({.primitive_type = ui::primitive_type::line}, nullptr, nullptr, nullptr)) {
    this->_node->add_sub_node(this->_triangle_node);
    this->_node->add_sub_node(this->_line_node);

    this->_triangle_node->set_mesh(this->_triangle_mesh);
    this->_line_node->set_mesh(this->_line_mesh);
    this->_line_node->set_color(ui::white_color());

    this->_set_rect_count(0);

    presenter->observe_modules([this](auto const &locations) { this->set_locations(locations); })
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<ui_modules> ui_modules::make_shared(std::string const &project_id) {
    auto const presenter = modules_presenter::make_shared(project_id);
    return std::shared_ptr<ui_modules>(new ui_modules{presenter});
}

std::shared_ptr<ui::node> const &ui_modules::node() const {
    return this->_node;
}

void ui_modules::set_scale(ui::size const &scale) {
    this->_node->set_scale(scale);
}

void ui_modules::set_locations(std::vector<module_location> const &locations) {
    this->_set_rect_count(locations.size());

    this->_write_vertices([&locations](vertex2d_rect *vertex_rects) {
        auto each = make_fast_each(locations.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &location = locations.at(idx);
            vertex_rects[idx].set_position(
                ui::region{.origin = {.x = location.x, .y = 0.0f}, .size = {.width = location.width, .height = 1.0f}});
        }
    });
}

void ui_modules::_remake_data_if_needed(std::size_t const max_count) {
    if (max_count <= this->_remaked_count) {
        return;
    }

    this->_vertex_data = nullptr;
    this->_triangle_index_data = nullptr;
    this->_line_index_data = nullptr;
    this->_triangle_mesh->set_vertex_data(nullptr);
    this->_triangle_mesh->set_index_data(nullptr);
    this->_line_mesh->set_vertex_data(nullptr);
    this->_line_mesh->set_index_data(nullptr);

    this->_vertex_data = ui::dynamic_mesh_vertex_data::make_shared(max_count * 4);
    this->_triangle_index_data = ui::dynamic_mesh_index_data::make_shared(max_count * 6);
    this->_line_index_data = ui::dynamic_mesh_index_data::make_shared(max_count * line_index2d_rect::vector_count);

    this->_write_vertices([&max_count](vertex2d_rect *vertex_rects) {
        auto rect_each = make_fast_each(max_count);
        while (yas_each_next(rect_each)) {
            auto const &rect_idx = yas_each_index(rect_each);
            auto &vertex_rect = vertex_rects[rect_idx];

            float const hue = float(rect_idx % 6) / 6.0f;
            auto const color = ui::hsb_color(hue, 1.0f, 1.0f);

            auto each = make_fast_each(vertex2d_rect::vector_count);
            while (yas_each_next(each)) {
                vertex_rect.v[yas_each_index(each)].color = to_float4(color, 0.3f);
            }
        }
    });

    this->_write_triangle_indices([&max_count](triangle_index2d_rect *index_rects) {
        auto each = make_fast_each(max_count);
        while (yas_each_next(each)) {
            auto const &rect_idx = yas_each_index(each);
            uint32_t const rect_head_idx = static_cast<uint32_t>(rect_idx * triangle_index2d_rect::vector_count);
            index_rects[rect_idx].set_all(rect_head_idx);
        }
    });

    this->_write_line_indices([&max_count](line_index2d_rect *index_rects) {
        auto each = make_fast_each(max_count);
        while (yas_each_next(each)) {
            auto const &rect_idx = yas_each_index(each);
            uint32_t const rect_head_idx = static_cast<uint32_t>(rect_idx * line_index2d_rect::vector_count);
            index_rects[rect_idx].set_all(rect_head_idx);
        }
    });

    this->_triangle_mesh->set_vertex_data(this->_vertex_data);
    this->_triangle_mesh->set_index_data(this->_triangle_index_data);
    this->_line_mesh->set_vertex_data(this->_vertex_data);
    this->_line_mesh->set_index_data(this->_line_index_data);

    this->_remaked_count = max_count;
}

void ui_modules::_set_rect_count(std::size_t const rect_count) {
    this->_remake_data_if_needed(common_utils::reserving_count(rect_count, ui_modules_constants::reserving_interval));

    this->_vertex_data->set_count(rect_count * vertex2d_rect::vector_count);
    this->_triangle_index_data->set_count(rect_count * triangle_index2d_rect::vector_count);
    this->_line_index_data->set_count(rect_count * line_index2d_rect::vector_count);
}

void ui_modules::_write_vertices(std::function<void(vertex2d_rect *)> const &handler) {
    this->_vertex_data->write([&handler](auto &vertices) { handler((ui::vertex2d_rect *)vertices.data()); });
}

void ui_modules::_write_triangle_indices(std::function<void(triangle_index2d_rect *)> const &handler) {
    this->_triangle_index_data->write([&handler](auto &indices) { handler((triangle_index2d_rect *)indices.data()); });
}

void ui_modules::_write_line_indices(std::function<void(line_index2d_rect *)> const &handler) {
    this->_line_index_data->write([&handler](auto &indices) { handler((line_index2d_rect *)indices.data()); });
}
