//
//  ae_ui_module_waveforms.mm
//

#include "ae_ui_module_waveforms.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_module_waveforms_presenter.h>
#include <audio_editor_core/ae_project_path.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_waveform_mesh_importer_types.h>
#include <cpp_utils/yas_fast_each.h>
#include <audio_editor_core/ae_ui_atlas.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct ui_module_waveforms::element {
    std::shared_ptr<ui::node> const node;

    element(std::shared_ptr<ui::node> &&node) : node(std::move(node)) {
    }

    void set_datas(std::vector<waveform_mesh_data> const &datas) {
        this->_datas = datas;
    }

    void clear() {
        this->node->remove_all_sub_nodes();
        this->_datas.clear();
    }

    void add_mesh_node(std::shared_ptr<ui::node> const &mesh_node) {
        this->node->add_sub_node(mesh_node);
    }

    void update_tex_coords(simd::float2 const &tex_coord) {
        for (auto const &mesh_data : this->_datas) {
            if (mesh_data.vertex_data) {
                mesh_data.vertex_data->write([&tex_coord](std::vector<ui::vertex2d_t> &vertices) {
                    for (auto &vertex : vertices) {
                        vertex.tex_coord = tex_coord;
                    }
                });
            }
        }
    }

    void update_colors(ui::color const &color) {
        for (auto const &mesh_node : this->_mesh_nodes()) {
            mesh_node->set_color(color);
        }
    }

    void finalize() {
        this->node->remove_from_super_node();
    }

   private:
    std::vector<waveform_mesh_data> _datas;

    std::vector<std::shared_ptr<ui::node>> const &_mesh_nodes() const {
        return this->node->sub_nodes();
    }
};
}

std::shared_ptr<ui_module_waveforms> ui_module_waveforms::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);
    auto const presenter = module_waveforms_presenter::make_shared(project_lifetime_id);

    return std::make_shared<ui_module_waveforms>(resource_lifetime->standard, app_lifetime->color.get(),
                                                 resource_lifetime->atlas.get(), presenter);
}

ui_module_waveforms::ui_module_waveforms(std::shared_ptr<ui::standard> const &standard, ae::color *color,
                                         ui_atlas const *atlas,
                                         std::shared_ptr<module_waveforms_presenter> const &presenter)
    : node(ui::node::make_shared()), _presenter(presenter), _color(color), _atlas(atlas) {
    this->_presenter
        ->observe_mesh_importer([this](waveform_mesh_importer_event const &event) {
            auto const &contents = this->_presenter->contents();

            if (event.index < this->_elements.size() && event.index < contents.size()) {
                auto const &content = contents.at(event.index);
                auto &element = this->_elements.at(event.index);
                if (element->node->is_enabled() && content.has_value()) {
                    element->clear();

                    auto const &waveform_color =
                        content.value().is_selected ? this->_color->selected_waveform() : this->_color->waveform();
                    auto const tex_coord = to_float2(this->_atlas->white_filled_tex_coords().origin);

                    auto each = make_fast_each(event.datas.size());
                    while (yas_each_next(each)) {
                        auto const &idx = yas_each_index(each);
                        auto const mesh_node = ui::node::make_shared();
                        auto const &vertex_data = event.datas.at(idx).vertex_data;

                        if (vertex_data != nullptr) {
                            vertex_data->write([&tex_coord](std::vector<ui::vertex2d_t> &vertices) {
                                for (auto &vertex : vertices) {
                                    vertex.tex_coord = tex_coord;
                                }
                            });
                        }

                        auto const mesh =
                            ui::mesh::make_shared({.primitive_type = ui::primitive_type::triangle}, vertex_data,
                                                  event.datas.at(idx).index_data, this->_atlas->texture());

                        mesh_node->set_color(waveform_color);
                        mesh_node->set_meshes({mesh});

                        element->add_mesh_node(mesh_node);
                    }

                    element->set_datas(event.datas);
                }
            }
        })
        .end()
        ->add_to(this->_pool);

    presenter
        ->observe_contents([this](module_content_pool_event const &event) {
            switch (event.type) {
                case module_content_pool_event_type::fetched:
                case module_content_pool_event_type::replaced:
                    this->_replace_elements(true);
                    break;
                case module_content_pool_event_type::updated:
                    this->_update_elements(event.elements.size(), event.erased, event.inserted_indices,
                                           event.replaced_indices);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) { this->_update_all_colors(); })
        .end()
        ->add_to(this->_pool);

    atlas
        ->observe_white_filled_tex_coords(
            [this](ui::uint_region const &tex_coord) { this->_update_all_tex_coords(tex_coord.origin); })
        .sync()
        ->add_to(this->_pool);
}

void ui_module_waveforms::set_scale(ui::size const &scale) {
    this->node->set_scale({.width = 1.0f, .height = scale.height});

    if (this->_scale != scale.width) {
        bool const prev_null = !this->_scale.has_value();

        this->_scale = scale.width;

        if (prev_null) {
            this->_replace_elements(false);
        }

        if (auto const waveform_scale = this->_waveform_scale()) {
            for (auto const &element : this->_elements) {
                element->node->set_scale(waveform_scale.value());
            }
        }
    }
}

void ui_module_waveforms::_replace_elements(bool const clear_meshes) {
    if (!this->_scale.has_value()) {
        this->_resize_elements(0);
        return;
    }

    auto const &contents = this->_presenter->contents();

    this->_resize_elements(contents.size());
    this->_update_all_colors();

    auto each = make_fast_each(contents.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &content = contents.at(idx);
        auto &element = this->_elements.at(idx);

        if (clear_meshes) {
            element->clear();
        }

        if (content.has_value()) {
            auto const &content_value = content.value();
            auto const region = content_value.region();
            element->node->set_is_enabled(true);
            element->node->set_position({.x = region.origin.x * content_value.scale.width, .y = region.center().y});
            this->_presenter->import(idx, content.value());
        } else {
            element->node->set_is_enabled(false);
        }
    }
}

void ui_module_waveforms::_update_elements(std::size_t const count, std::map<std::size_t, module_content> const &erased,
                                           std::set<std::size_t> const &inserted_indices,
                                           std::set<std::size_t> const &replaced_indices) {
    if (!this->_scale.has_value()) {
        this->_resize_elements(0);
        this->_elements.clear();
        return;
    }

    this->_resize_elements(count);

    auto const &contents = this->_presenter->contents();

    for (auto const &pair : erased) {
        auto const &idx = pair.first;
        auto const &content = pair.second;

        if (idx < this->_elements.size()) {
            auto &element = this->_elements.at(idx);
            element->clear();
            element->node->set_is_enabled(false);
        }

        this->_presenter->cancel_import(content.identifier);
    }

    auto const normal_color = this->_color->waveform();
    auto const selected_color = this->_color->selected_waveform();

    for (auto const &idx : replaced_indices) {
        if (idx < this->_elements.size() && idx < contents.size()) {
            auto const &content = contents.at(idx).value();
            auto const region = content.region();
            auto &element = this->_elements.at(idx);
            element->node->set_is_enabled(true);
            element->node->set_position({.x = region.origin.x * content.scale.width, .y = region.center().y});
            auto const &color = content.is_selected ? selected_color : normal_color;
            element->update_colors(color);
            this->_presenter->import(idx, content);
        }
    }

    for (auto const &idx : inserted_indices) {
        if (idx < this->_elements.size()) {
            auto const &content = contents.at(idx).value();
            auto const region = content.region();
            auto &element = this->_elements.at(idx);
            element->clear();
            element->node->set_is_enabled(true);
            element->node->set_position({.x = region.origin.x * content.scale.width, .y = region.center().y});
            auto const &color = content.is_selected ? selected_color : normal_color;
            element->update_colors(color);
            this->_presenter->import(idx, content);
        }
    }
}

void ui_module_waveforms::_resize_elements(std::size_t const count) {
    auto const prev_count = this->_elements.size();

    if (prev_count < count) {
        auto const scale = this->_waveform_scale();

        auto each = make_fast_each(count - prev_count);
        while (yas_each_next(each)) {
            auto element_node = ui::node::make_shared();
            element_node->set_is_enabled(false);

            if (scale.has_value()) {
                element_node->set_scale(scale.value());
            }

            this->node->add_sub_node(element_node);
            this->_elements.emplace_back(std::make_shared<element>(std::move(element_node)));
        }
    } else if (prev_count > count) {
        auto each = make_fast_each(prev_count - count);
        while (yas_each_next(each)) {
            auto const idx = prev_count - yas_each_index(each) - 1;
            this->_elements.at(idx)->finalize();
            yas::erase_at(this->_elements, idx);
        }
    }
}

std::optional<ui::size> ui_module_waveforms::_waveform_scale() const {
    if (this->_scale.has_value()) {
        float const scale = this->_scale.value();
        return ui::size{.width = scale, .height = 1.0f};
    } else {
        return std::nullopt;
    }
}

void ui_module_waveforms::_update_all_tex_coords(ui::uint_point const &tex_coord_origin) {
    auto const tex_coord = to_float2(tex_coord_origin);

    for (auto &element : this->_elements) {
        element->update_tex_coords(tex_coord);
    }
}

void ui_module_waveforms::_update_all_colors() {
    auto const normal_color = this->_color->waveform();
    auto const selected_color = this->_color->selected_waveform();
    auto const &contents = this->_presenter->contents();
    auto const &elements = this->_elements;
    auto const contents_size = std::min(contents.size(), this->_elements.size());

    auto each = make_fast_each(contents_size);
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &content = contents.at(idx);
        auto const &element = elements.at(idx);

        if (content.has_value()) {
            auto const &color = content.value().is_selected ? selected_color : normal_color;
            element->update_colors(color);
        }
    }
}
