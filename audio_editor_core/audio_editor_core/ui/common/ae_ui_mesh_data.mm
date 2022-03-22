//
//  ae_ui_mesh_data.mm
//

#include "ae_ui_mesh_data.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_mesh_data> ui_mesh_data::make_shared(ui::primitive_type const primitive_type,
                                                        std::shared_ptr<ui::static_mesh_vertex_data> const &vertex_data,
                                                        std::shared_ptr<ui::static_mesh_index_data> const &index_data) {
    return std::shared_ptr<ui_mesh_data>(new ui_mesh_data{primitive_type, vertex_data, index_data});
}

ui_mesh_data::ui_mesh_data(ui::primitive_type const primitive_type,
                           std::shared_ptr<ui::static_mesh_vertex_data> const &vertex_data,
                           std::shared_ptr<ui::static_mesh_index_data> const &index_data)
    : primitive_type(primitive_type), vertex_data(vertex_data), index_data(index_data) {
}
