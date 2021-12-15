//
//  ae_ui_module_waveforms.mm
//

#include "ae_ui_module_waveforms.h"
#include <audio_editor_core/ae_waveform_mesh_importer.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_module_waveforms> ui_module_waveforms::make_shared() {
    return std::shared_ptr<ui_module_waveforms>(new ui_module_waveforms{});
}

ui_module_waveforms::ui_module_waveforms()
    : _node(ui::node::make_shared()), _mesh_importer(waveform_mesh_importer::make_shared()) {
    this->_mesh_importer
        ->observe([this](waveform_mesh_importer::event const &event) {
            auto const &sub_nodes = this->_node->children();
            if (event.index < sub_nodes.size()) {
                auto const &sub_node = sub_nodes.at(event.index);
                if (sub_node->is_enabled()) {
                    auto const &mesh = sub_node->mesh();
                    mesh->set_vertex_data(event.mesh_vertex_data);
                    mesh->set_index_data(event.mesh_index_data);
                }
            }
        })
        .end()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::node> const &ui_module_waveforms::node() {
    return this->_node;
}

void ui_module_waveforms::set_locations(std::vector<std::optional<module_location>> const &locations) {
    this->_resize_sub_nodes(locations.size());

    auto const sub_nodes = this->_node->children();

    auto each = make_fast_each(locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &location = locations.at(idx);
        auto const &sub_node = sub_nodes.at(idx);
        auto const &mesh = sub_node->mesh();
        mesh->set_vertex_data(nullptr);
        mesh->set_index_data(nullptr);

        if (location.has_value()) {
            sub_node->set_is_enabled(true);
            sub_node->set_position({.x = location.value().x, .y = 0.0f});
            this->_mesh_importer->import(idx, location.value());
        } else {
            sub_node->set_is_enabled(false);
        }
    }
}

void ui_module_waveforms::update_locations(std::size_t const count,
                                           std::vector<std::pair<std::size_t, module_location>> const &erased,
                                           std::vector<std::pair<std::size_t, module_location>> const &inserted) {
    this->_resize_sub_nodes(count);

    auto each = make_fast_each(erased.size());
    while (yas_each_next(each)) {
        auto const &pair = erased.at(yas_each_index(each));
        auto const &idx = pair.first;
        auto const &location = pair.second;

        if (idx < this->_node->children().size()) {
            auto const &sub_node = this->_node->children().at(idx);
            auto const &mesh = sub_node->mesh();
            mesh->set_vertex_data(nullptr);
            mesh->set_index_data(nullptr);
            sub_node->set_is_enabled(false);
        }

        this->_mesh_importer->cancel(location.identifier);
    }

    each = make_fast_each(inserted.size());
    while (yas_each_next(each)) {
        auto const &pair = inserted.at(yas_each_index(each));
        auto const &idx = pair.first;

        if (idx < this->_node->children().size()) {
            auto const &location = pair.second;
            auto const &sub_node = this->_node->children().at(idx);
            auto const &mesh = sub_node->mesh();
            mesh->set_vertex_data(nullptr);
            mesh->set_index_data(nullptr);
            sub_node->set_is_enabled(true);
            sub_node->set_position({.x = location.x, .y = 0.0f});
            this->_mesh_importer->import(idx, location);
        }
    }
}

void ui_module_waveforms::_resize_sub_nodes(std::size_t const count) {
    auto const &prev_count = this->_node->children().size();

    if (prev_count < count) {
        auto each = make_fast_each(count - prev_count);
        while (yas_each_next(each)) {
            auto const node = ui::node::make_shared();
            node->set_is_enabled(false);
            auto const mesh = ui::mesh::make_shared();
            mesh->set_primitive_type(ui::primitive_type::line);
            node->set_mesh(mesh);
            this->_node->add_sub_node(node);
        }
    } else if (prev_count > count) {
        auto each = make_fast_each(prev_count - count);
        while (yas_each_next(each)) {
            auto const idx = prev_count - yas_each_index(each) - 1;
            this->_node->children().at(idx)->remove_from_super_node();
        }
    }
}
