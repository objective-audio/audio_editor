//
//  ae_ui_markers.mm
//

#include "ae_ui_markers.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_markers_presenter.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_root_level.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_markers_constants {
static std::size_t const reserving_interval = 10;
}

std::shared_ptr<ui_markers> ui_markers::make_shared(ui_project_id const &project_id) {
    auto const &app_level = app_level::global();
    auto const &ui_root_level = ui_hierarchy::root_level_for_view_id(project_id.view_id);

    auto const presenter = markers_presenter::make_shared(project_id.identifier, ui_root_level->display_space);
    auto const &color = app_level->color;
    return std::shared_ptr<ui_markers>(new ui_markers{presenter, ui_root_level->standard, color});
}

ui_markers::ui_markers(std::shared_ptr<markers_presenter> const &presenter,
                       std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ae::color> const &color)
    : node(ui::node::make_shared()),
      _presenter(presenter),
      _color(color),
      _top_guide(ui::layout_value_guide::make_shared()),
      _vertex_data(ui::static_mesh_vertex_data::make_shared(3)),
      _index_data(ui::static_mesh_index_data::make_shared(3)) {
    this->node->set_batch(ui::batch::make_shared());

    this->_vertex_data->write_once([](std::vector<ui::vertex2d_t> &vertices) {
        float const half_width = -5.0f;
        float const height = 10.0f;
        vertices[0].position = {0.0f, -height};
        vertices[1].position = {-half_width, 0.0f};
        vertices[2].position = {half_width, 0.0f};
    });

    this->_index_data->write_once([](std::vector<ui::index2d_t> &indices) {
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
    });

    this->_presenter
        ->observe_locations([this](marker_location_pool_event const &event) {
            switch (event.type) {
                case marker_location_pool_event_type::fetched:
                case marker_location_pool_event_type::replaced:
                    this->set_locations(event.elements);
                    break;
                case marker_location_pool_event_type::updated:
                    this->update_locations(event.elements.size(), event.erased, event.inserted);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) { this->_presenter->update_if_needed(); })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) {
            auto const &marker_color = this->_color->marker();

            for (auto const &sub_node : this->_sub_nodes) {
                sub_node->set_color(marker_color);
            }
        })
        .sync()
        ->add_to(this->_pool);

    ui::layout(standard->view_look()->view_layout_guide()->top(), this->_top_guide, ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);
}

void ui_markers::set_locations(std::vector<std::optional<marker_location>> const &locations) {
    this->_set_count(locations.size());

    auto each = make_fast_each(locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &location = locations.at(idx);
        auto const &node = this->_sub_nodes.at(idx);
        if (location.has_value()) {
            auto const &location_value = location.value();
            node->set_is_enabled(true);
            node->set_position({location_value.x, node->position().y});
        } else {
            node->set_is_enabled(false);
        }
    }
}

void ui_markers::update_locations(std::size_t const count,
                                  std::vector<std::pair<std::size_t, marker_location>> const &erased,
                                  std::vector<std::pair<std::size_t, marker_location>> const &inserted) {
    this->_set_count(count);

    for (auto const &pair : erased) {
        auto const &idx = pair.first;
        if (idx < count) {
            this->_sub_nodes.at(idx)->set_is_enabled(false);
        }
    }

    for (auto const &pair : inserted) {
        auto const &idx = pair.first;
        auto const &location = pair.second;
        auto const &node = this->_sub_nodes.at(idx);
        node->set_is_enabled(true);
        node->set_position({location.x, node->position().y});
    }
}

void ui_markers::_set_count(std::size_t const location_count) {
    auto const &marker_color = this->_color->marker();
    auto const prev_node_count = this->_sub_nodes.size();

    if (prev_node_count < location_count) {
        this->_sub_nodes.reserve(
            common_utils::reserving_count(location_count, ui_markers_constants::reserving_interval));

        auto each = make_fast_each(location_count - prev_node_count);
        while (yas_each_next(each)) {
            auto node = ui::node::make_shared();
            node->set_mesh(ui::mesh::make_shared({}, this->_vertex_data, this->_index_data, nullptr));
            node->set_color(marker_color);
            node->set_is_enabled(false);
            node->attach_y_layout_guide(*this->_top_guide);
            this->node->add_sub_node(node);
            this->_sub_nodes.emplace_back(std::move(node));
        }
    } else if (location_count < prev_node_count) {
        auto each = make_fast_each(prev_node_count - location_count);
        while (yas_each_next(each)) {
            auto const idx = prev_node_count - 1 - yas_each_index(each);
            this->_sub_nodes.at(idx)->remove_from_super_node();
        }
        this->_sub_nodes.resize(location_count);
    }
}
