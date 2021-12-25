//
//  ae_ui_module_waveforms.mm
//

#include "ae_ui_module_waveforms.h"
#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_module_waveforms_presenter.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_waveform_mesh_importer.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_module_waveforms> ui_module_waveforms::make_shared(
    std::shared_ptr<module_waveforms_presenter> const &presenter) {
    return std::shared_ptr<ui_module_waveforms>(new ui_module_waveforms{presenter});
}

ui_module_waveforms::ui_module_waveforms(std::shared_ptr<module_waveforms_presenter> const &presenter)
    : _presenter(presenter), _node(ui::node::make_shared()) {
    this->_presenter
        ->observe_mesh_importer([this](waveform_mesh_importer_event const &event) {
            auto const &sub_nodes = this->_node->sub_nodes();
            if (event.index < sub_nodes.size()) {
                auto const &sub_node = sub_nodes.at(event.index);
                if (sub_node->is_enabled()) {
                    sub_node->remove_all_sub_nodes();

                    auto each = make_fast_each(event.datas.size());
                    while (yas_each_next(each)) {
                        auto const &idx = yas_each_index(each);
                        auto const mesh_node = ui::node::make_shared();
                        auto const mesh = ui::mesh::make_shared();
                        mesh->set_primitive_type(ui::primitive_type::triangle);
                        mesh->set_vertex_data(event.datas.at(idx).vertex_data);
                        mesh->set_index_data(event.datas.at(idx).index_data);
                        mesh_node->set_mesh(mesh);
                        sub_node->add_sub_node(mesh_node);
                    }
                }
            }
        })
        .end()
        ->add_to(this->_pool);

    presenter
        ->observe_locations([this](module_location_pool_event const &event) {
            switch (event.type) {
                case module_location_pool_event_type::fetched:
                case module_location_pool_event_type::replaced:
                    this->set_locations(event.elements);
                    break;
                case module_location_pool_event_type::updated:
                    this->update_locations(event.elements.size(), event.erased, event.inserted);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::node> const &ui_module_waveforms::node() {
    return this->_node;
}

void ui_module_waveforms::set_scale(ui::size const &scale) {
    this->_node->set_scale({.width = 1.0f, .height = scale.height * 0.5f});

    if (this->_width_per_sec != scale.width) {
        this->_width_per_sec = scale.width;
        this->set_locations(this->_presenter->locations());
    }
}

void ui_module_waveforms::set_locations(std::vector<std::optional<module_location>> const &locations) {
    if (!this->_width_per_sec.has_value()) {
        this->_resize_sub_nodes(0);
        return;
    }

    auto const width_per_sec = this->_width_per_sec.value();

    this->_resize_sub_nodes(locations.size());

    auto const sub_nodes = this->_node->sub_nodes();

    auto each = make_fast_each(locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &location = locations.at(idx);
        auto const &sub_node = sub_nodes.at(idx);

        sub_node->remove_all_sub_nodes();

        if (location.has_value()) {
            sub_node->set_is_enabled(true);
            sub_node->set_position({.x = location.value().x * width_per_sec, .y = 0.0f});
            this->_presenter->import(idx, location.value(), width_per_sec);
        } else {
            sub_node->set_is_enabled(false);
        }
    }
}

void ui_module_waveforms::update_locations(std::size_t const count,
                                           std::vector<std::pair<std::size_t, module_location>> const &erased,
                                           std::vector<std::pair<std::size_t, module_location>> const &inserted) {
    if (!this->_width_per_sec.has_value()) {
        this->_resize_sub_nodes(0);
        return;
    }

    auto const width_per_sec = this->_width_per_sec.value();

    this->_resize_sub_nodes(count);

    auto each = make_fast_each(erased.size());
    while (yas_each_next(each)) {
        auto const &pair = erased.at(yas_each_index(each));
        auto const &idx = pair.first;
        auto const &location = pair.second;

        if (idx < this->_node->sub_nodes().size()) {
            auto const &sub_node = this->_node->sub_nodes().at(idx);
            sub_node->remove_all_sub_nodes();
            sub_node->set_is_enabled(false);
        }

        this->_presenter->cancel_import(location.identifier);
    }

    each = make_fast_each(inserted.size());
    while (yas_each_next(each)) {
        auto const &pair = inserted.at(yas_each_index(each));
        auto const &idx = pair.first;

        if (idx < this->_node->sub_nodes().size()) {
            auto const &location = pair.second;
            auto const &sub_node = this->_node->sub_nodes().at(idx);
            sub_node->remove_all_sub_nodes();
            sub_node->set_is_enabled(true);
            sub_node->set_position({.x = location.x * width_per_sec, .y = 0.0f});
            this->_presenter->import(idx, location, width_per_sec);
        }
    }
}

void ui_module_waveforms::_resize_sub_nodes(std::size_t const count) {
    auto const &prev_count = this->_node->sub_nodes().size();

    if (prev_count < count) {
        auto each = make_fast_each(count - prev_count);
        while (yas_each_next(each)) {
            auto const node = ui::node::make_shared();
            node->set_is_enabled(false);
            this->_node->add_sub_node(node);
        }
    } else if (prev_count > count) {
        auto each = make_fast_each(prev_count - count);
        while (yas_each_next(each)) {
            auto const idx = prev_count - yas_each_index(each) - 1;
            this->_node->sub_nodes().at(idx)->remove_from_super_node();
        }
    }
}