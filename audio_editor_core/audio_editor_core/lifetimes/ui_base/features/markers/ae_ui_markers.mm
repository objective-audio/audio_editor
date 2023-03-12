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
#include <audio_editor_core/ae_markers_controller.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_markers_constants {
static std::size_t const reserving_interval = 10;
}

std::shared_ptr<ui_markers> ui_markers::make_shared(window_lifetime_id const &window_lifetime_id,
                                                    std::shared_ptr<ui::node> const &node) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(window_lifetime_id);

    auto const presenter = markers_presenter::make_shared(window_lifetime_id, project_lifetime->marker_content_pool);
    auto const controller = markers_controller::make_shared(window_lifetime_id);
    return std::make_shared<ui_markers>(window_lifetime_id, presenter, controller, resource_lifetime->standard,
                                        node.get());
}

ui_markers::ui_markers(window_lifetime_id const &window_lifetime_id,
                       std::shared_ptr<markers_presenter> const &presenter,
                       std::shared_ptr<markers_controller> const &controller,
                       std::shared_ptr<ui::standard> const &standard, ui::node *node)
    : _window_lifetime_id(window_lifetime_id),
      _node(node),
      _presenter(presenter),
      _controller(controller),
      _top_guide(standard->view_look()->view_layout_guide()->top()) {
    this->_presenter
        ->observe_contents([this](marker_content_pool_event const &event) {
            switch (event.type) {
                case marker_content_pool_event_type::fetched:
                case marker_content_pool_event_type::replaced:
                    this->_replace(event.elements);
                    break;
                case marker_content_pool_event_type::updated:
                    this->_update(event.elements.size(), event.erased, event.inserted, event.replaced);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) { this->_presenter->update_if_needed(); })
        .end()
        ->add_to(this->_pool);

    presenter
        ->observe_range_selection_region([this](std::optional<ui::region> const &rect) {
            if (rect.has_value()) {
                auto const &rect_value = rect.value();
                std::vector<marker_index> hit_indices;

                auto each = make_fast_each(this->_elements.size());
                while (yas_each_next(each)) {
                    auto const &idx = yas_each_index(each);
                    auto const &element = this->_elements.at(idx);
                    if (element->hit_test(rect_value)) {
                        if (auto const marker_index = element->marker_index(); marker_index.has_value()) {
                            hit_indices.emplace_back(marker_index.value());
                        }
                    }
                }

                this->_controller->select(hit_indices);
            }
        })
        .sync()
        ->add_to(this->_pool);
}

void ui_markers::_replace(std::vector<std::optional<marker_content>> const &contents) {
    this->_set_count(contents.size());

    auto each = make_fast_each(contents.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &content = contents.at(idx);
        auto const &element = this->_elements.at(idx);
        if (content.has_value()) {
            auto const &content_value = content.value();
            element->set_content(content_value);
        } else {
            element->reset_content();
        }
    }
}

void ui_markers::_update(std::size_t const count, std::vector<std::pair<std::size_t, marker_content>> const &erased,
                         std::vector<std::pair<std::size_t, marker_content>> const &inserted,
                         std::vector<std::pair<std::size_t, marker_content>> const &replaced) {
    this->_set_count(count);

    for (auto const &pair : erased) {
        auto const &idx = pair.first;
        if (idx < count) {
            this->_elements.at(idx)->reset_content();
        }
    }

    for (auto const &pair : inserted) {
        auto const &idx = pair.first;
        auto const &content = pair.second;
        auto const &element = this->_elements.at(idx);
        element->set_content(content);
    }

    for (auto const &pair : replaced) {
        auto const &idx = pair.first;
        auto const &content = pair.second;
        auto const &element = this->_elements.at(idx);
        element->update_content(content);
    }
}

void ui_markers::_set_count(std::size_t const content_count) {
    auto const prev_element_count = this->_elements.size();

    if (prev_element_count < content_count) {
        this->_elements.reserve(common_utils::reserving_count(content_count, ui_markers_constants::reserving_interval));

        auto each = make_fast_each(content_count - prev_element_count);
        while (yas_each_next(each)) {
            auto element = ui_marker_element::make_shared(this->_window_lifetime_id, this->_node);
            this->_elements.emplace_back(std::move(element));
        }
    } else if (content_count < prev_element_count) {
        auto each = make_fast_each(prev_element_count - content_count);
        while (yas_each_next(each)) {
            auto const idx = prev_element_count - 1 - yas_each_index(each);
            this->_elements.at(idx)->finalize();
        }
        this->_elements.resize(content_count);
    }
}
