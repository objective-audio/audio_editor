//
//  ae_ui_markers.mm
//

#include "ae_ui_markers.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_markers_presenter.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_marker_element.h>
#include <audio_editor_core/ae_ui_mesh_data.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_markers_constants {
static std::size_t const reserving_interval = 10;
}

std::shared_ptr<ui_markers> ui_markers::make_shared(window_lifetime_id const &window_lifetime_id,
                                                    std::shared_ptr<ae::display_space> const &display_space,
                                                    std::shared_ptr<marker_location_pool> const &location_pool,
                                                    std::shared_ptr<ui::standard> const &standard,
                                                    std::shared_ptr<ui::font_atlas> const &font_atlas,
                                                    std::shared_ptr<ui_mesh_data> const &vertical_line_data) {
    auto const &app_lifetime = hierarchy::app_lifetime();

    auto const presenter = markers_presenter::make_shared(window_lifetime_id, display_space, location_pool);
    auto const &color = app_lifetime->color;
    return std::make_shared<ui_markers>(presenter, standard, font_atlas, color, vertical_line_data);
}

ui_markers::ui_markers(std::shared_ptr<markers_presenter> const &presenter,
                       std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ui::font_atlas> const &font_atlas,
                       std::shared_ptr<ae::color> const &color, std::shared_ptr<ui_mesh_data> const &vertical_line_data)
    : node(ui::node::make_shared()),
      _standard(standard),
      _font_atlas(font_atlas),
      _presenter(presenter),
      _color(color),
      _top_guide(standard->view_look()->view_layout_guide()->top()),
      _vertical_line_data(vertical_line_data),
      _triangle_data(ui_mesh_data::make_shared(ui::primitive_type::triangle,
                                               ui::static_mesh_vertex_data::make_shared(3),
                                               ui::static_mesh_index_data::make_shared(3))) {
    this->node->set_batch(ui::batch::make_shared());

    this->_triangle_data->vertex_data->write_once([](std::vector<ui::vertex2d_t> &vertices) {
        float const half_width = -5.0f;
        float const height = 10.0f;
        vertices[0].position = {0.0f, -height};
        vertices[1].position = {-half_width, 0.0f};
        vertices[2].position = {half_width, 0.0f};
    });

    this->_triangle_data->index_data->write_once([](std::vector<ui::index2d_t> &indices) {
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
}

void ui_markers::set_locations(std::vector<std::optional<marker_location>> const &locations) {
    this->_set_count(locations.size());

    auto each = make_fast_each(locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &location = locations.at(idx);
        auto const &node = this->_elements.at(idx)->node;
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
            this->_elements.at(idx)->node->set_is_enabled(false);
        }
    }

    for (auto const &pair : inserted) {
        auto const &idx = pair.first;
        auto const &location = pair.second;
        auto const &node = this->_elements.at(idx)->node;
        node->set_is_enabled(true);
        node->set_position({location.x, node->position().y});
    }
}

void ui_markers::_set_count(std::size_t const location_count) {
    auto const prev_element_count = this->_elements.size();

    if (prev_element_count < location_count) {
        ui_marker_element::args const args{.vertical_line_data = this->_vertical_line_data,
                                           .triangle_data = this->_triangle_data};

        this->_elements.reserve(
            common_utils::reserving_count(location_count, ui_markers_constants::reserving_interval));

        auto each = make_fast_each(location_count - prev_element_count);
        while (yas_each_next(each)) {
            auto element = ui_marker_element::make_shared(args, this->_standard, this->_font_atlas);
            element->node->set_is_enabled(false);
            this->node->add_sub_node(element->node);
            this->_elements.emplace_back(std::move(element));
        }
    } else if (location_count < prev_element_count) {
        auto each = make_fast_each(prev_element_count - location_count);
        while (yas_each_next(each)) {
            auto const idx = prev_element_count - 1 - yas_each_index(each);
            this->_elements.at(idx)->node->remove_from_super_node();
        }
        this->_elements.resize(location_count);
    }
}
