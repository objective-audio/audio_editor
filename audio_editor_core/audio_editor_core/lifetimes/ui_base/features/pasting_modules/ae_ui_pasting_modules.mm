//
//  ae_ui_pasting_modules.mm
//

#include "ae_ui_pasting_modules.hpp"

#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_types.h>
#include <audio_editor_core/ae_pasting_modules_presenter.hpp>
#include <audio_editor_core/ae_ui_dynamic_mesh_container.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_pasting_modules_constants {
static std::size_t const reserving_interval = 16;
}

namespace yas::ae::ui_pasting_modules_utils {
static std::unique_ptr<dynamic_mesh_content> make_element(std::size_t const idx, std::size_t const element_count) {
    auto vertex_data = ui::dynamic_mesh_vertex_data::make_shared(element_count * vertex2d_rect::vector_count);
    auto index_data = ui::dynamic_mesh_index_data::make_shared(element_count * frame_index2d_rect::vector_count);
    auto mesh = ui::mesh::make_shared({.primitive_type = ui::primitive_type::line}, vertex_data, index_data, nullptr);

    return std::unique_ptr<dynamic_mesh_content>(new dynamic_mesh_content{
        .vertex_data = std::move(vertex_data), .index_data = std::move(index_data), .mesh = std::move(mesh)});
}
}

#pragma mark - ui_pasting_modules

std::shared_ptr<ui_pasting_modules> ui_pasting_modules::make_shared(project_lifetime_id const &project_lifetime_id,
                                                                    ui::node *node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);

    auto const modules_presenter = pasting_modules_presenter::make_shared(project_lifetime_id);
    return std::make_shared<ui_pasting_modules>(modules_presenter, resource_lifetime->standard, node,
                                                app_lifetime->color.get());
}

ui_pasting_modules::ui_pasting_modules(std::shared_ptr<pasting_modules_presenter> const &presenter,
                                       std::shared_ptr<ui::standard> const &standard, ui::node *node, ae::color *color)
    : _presenter(presenter),
      _node(node),
      _color(color),
      _scale_node(ui::node::make_shared()),
      _mesh_container(std::make_unique<dynamic_mesh_container<vertex2d_rect, frame_index2d_rect>>(
          ui_pasting_modules_constants::reserving_interval, ui_pasting_modules_utils::make_element)) {
    node->add_sub_node(this->_scale_node);
    this->_scale_node->add_sub_node(this->_mesh_container->node);

    presenter
        ->observe_contents([this](pasting_module_content_pool_event const &event) {
            switch (event.type) {
                case pasting_module_content_pool_event_type::fetched:
                case pasting_module_content_pool_event_type::replaced:
                    this->_replace();
                    break;
                case pasting_module_content_pool_event_type::updated:
                    this->_update_mesh(event.inserted_indices, event.replaced_indices, event.erased);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) {
            this->_presenter->update_if_needed();
            this->_update_y_offset();
        })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance(
            [this](auto const &) { this->_mesh_container->node->set_color(this->_color->pasting_module_frame()); })
        .sync()
        ->add_to(this->_pool);
}

void ui_pasting_modules::set_scale(ui::size const &scale) {
    this->_scale_node->set_scale(scale);
}

void ui_pasting_modules::_replace() {
    auto const &contents = this->_presenter->contents();

    this->_mesh_container->set_element_count(contents.size());

    this->_mesh_container->write_vertex_elements([&contents](index_range const range, ui::vertex2d_rect *vertex_rects) {
        if (contents.size() <= range.index) {
            return;
        }

        auto const process_length = std::min(range.length, contents.size() - range.index);

        auto each = make_fast_each(process_length);
        while (yas_each_next(each)) {
            auto const &vertex_idx = yas_each_index(each);
            auto const content_idx = range.index + vertex_idx;
            auto const &content = contents.at(content_idx);

            if (content.has_value()) {
                auto const region = content.value().region();
                vertex_rects[vertex_idx].set_position(region);
            } else {
                vertex_rects[vertex_idx].set_position(ui::region::zero());
            }
        }
    });
}

void ui_pasting_modules::_update_mesh(std::set<std::size_t> const &inserted_indices,
                                      std::set<std::size_t> const &replaced_indices,
                                      std::map<std::size_t, pasting_module_content> const &erased) {
    auto const &contents = this->_presenter->contents();

    this->_mesh_container->set_element_count(contents.size());

    this->_mesh_container->write_vertex_elements(
        [&contents, &erased, &inserted_indices](index_range const range, ui::vertex2d_rect *vertex_rects) {
            for (auto const &pair : erased) {
                auto const &content_idx = pair.first;
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    vertex_rects[vertex_idx].set_position(ui::region::zero());
                }
            }

            for (auto const &content_idx : inserted_indices) {
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &content = contents.at(content_idx).value();
                    auto const &region = content.region();
                    vertex_rects[vertex_idx].set_position(region);
                }
            }
        });
}

void ui_pasting_modules::_update_y_offset() {
    this->_mesh_container->node->set_position(
        ui::point{.x = 0.0f, .y = static_cast<float>(this->_presenter->y_offset())});
}
