//
//  ae_ui_markers.mm
//

#include "ae_ui_markers.h"
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
                                                    std::shared_ptr<ui::node> const &node) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(window_lifetime_id);

    auto const presenter = markers_presenter::make_shared(window_lifetime_id, resource_lifetime->display_space,
                                                          project_lifetime->marker_location_pool);
    return std::make_shared<ui_markers>(window_lifetime_id, presenter, resource_lifetime->standard, node.get());
}

ui_markers::ui_markers(window_lifetime_id const &window_lifetime_id,
                       std::shared_ptr<markers_presenter> const &presenter,
                       std::shared_ptr<ui::standard> const &standard, ui::node *node)
    : _window_lifetime_id(window_lifetime_id),
      _node(node),
      _presenter(presenter),
      _top_guide(standard->view_look()->view_layout_guide()->top()) {
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
        this->_elements.reserve(
            common_utils::reserving_count(location_count, ui_markers_constants::reserving_interval));

        auto each = make_fast_each(location_count - prev_element_count);
        while (yas_each_next(each)) {
            auto element = ui_marker_element::make_shared(this->_window_lifetime_id);
            element->node->set_is_enabled(false);
            this->_node->add_sub_node(element->node);
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
