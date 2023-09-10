//
//  ae_ui_mesh_utils.mm
//

#include "ae_ui_mesh_utils.hpp"

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_mesh_utils {
std::unique_ptr<dynamic_mesh_container<vertex2d_rect, fill_index2d_rect>> make_fill_container(
    std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>> *vertex_datas, std::size_t const interval) {
    auto make_fill_element = [vertex_datas](std::size_t const idx, std::size_t const element_count) {
        if (vertex_datas->size() == idx) {
            vertex_datas->emplace_back(
                ui::dynamic_mesh_vertex_data::make_shared(element_count * vertex2d_rect::vector_count));
        } else {
            assert(0);
        }

        auto const &vertex_data = vertex_datas->at(idx);
        auto const index_data =
            ui::dynamic_mesh_index_data::make_shared(element_count * fill_index2d_rect::vector_count);
        auto mesh = ui::mesh::make_shared({.primitive_type = ui::primitive_type::triangle, .use_mesh_color = true},
                                          vertex_data, index_data, nullptr);

        return std::unique_ptr<dynamic_mesh_content>(new dynamic_mesh_content{
            .vertex_data = vertex_data, .index_data = std::move(index_data), .mesh = std::move(mesh)});
    };

    return std::make_unique<dynamic_mesh_container<vertex2d_rect, fill_index2d_rect>>(interval,
                                                                                      std::move(make_fill_element));
}

std::unique_ptr<dynamic_mesh_container<vertex2d_rect, frame_index2d_rect>> make_frame_container(
    std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>> *vertex_datas, std::size_t const interval) {
    auto make_frame_element = [vertex_datas](std::size_t const idx, std::size_t const element_count) {
        auto const &vertex_data = vertex_datas->at(idx);
        auto const index_data =
            ui::dynamic_mesh_index_data::make_shared(element_count * frame_index2d_rect::vector_count);
        auto mesh =
            ui::mesh::make_shared({.primitive_type = ui::primitive_type::line}, vertex_data, index_data, nullptr);

        return std::unique_ptr<dynamic_mesh_content>(new dynamic_mesh_content{
            .vertex_data = vertex_data, .index_data = std::move(index_data), .mesh = std::move(mesh)});
    };

    return std::make_unique<dynamic_mesh_container<vertex2d_rect, frame_index2d_rect>>(interval,
                                                                                       std::move(make_frame_element));
}
}
