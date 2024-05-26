//
//  ui_mesh_data.mm
//

#include "ui_mesh_data.h"

using namespace yas;
using namespace yas::ae;

ui_mesh_data::ui_mesh_data(ui::primitive_type const primitive_type,
                           std::shared_ptr<ui::mesh_vertex_data> const &vertex_data,
                           std::shared_ptr<ui::mesh_index_data> const &index_data)
    : primitive_type(primitive_type), vertex_data(vertex_data), index_data(index_data) {
}
