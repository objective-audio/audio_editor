//
//  ae_pasting_markers.cpp
//

#include "ae_ui_pasting_markers.hpp"
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_pasting_markers_presenter.hpp>
#include <audio_editor_core/ae_ui_pasting_marker_element.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_pasting_markers_constants {
static std::size_t const reserving_interval = 16;
}

std::shared_ptr<ui_pasting_markers> ui_pasting_markers::make_shared(project_lifetime_id const &project_lifetime_id,
                                                                    std::shared_ptr<ui::node> const &node) {
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);
    auto const presenter = pasting_markers_presenter::make_shared(project_lifetime_id);
    return std::make_shared<ui_pasting_markers>(project_lifetime_id, presenter, resource_lifetime->standard,
                                                node.get());
}

ui_pasting_markers::ui_pasting_markers(project_lifetime_id const &project_lifetime_id,
                                       std::shared_ptr<pasting_markers_presenter> const &presenter,
                                       std::shared_ptr<ui::standard> const &standard, ui::node *node)
    : _project_lifetime_id(project_lifetime_id), _presenter(presenter), _node(node) {
    this->_presenter
        ->observe_contents([this](pasting_marker_content_pool_event const &event) {
            switch (event.type) {
                case pasting_marker_content_pool_event_type::fetched:
                case pasting_marker_content_pool_event_type::replaced:
                    this->_replace(event.elements);
                    break;
                case pasting_marker_content_pool_event_type::updated:
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
}

void ui_pasting_markers::_replace(std::vector<std::optional<pasting_marker_content>> const &contents) {
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

void ui_pasting_markers::_update(std::size_t const count,
                                 std::vector<std::pair<std::size_t, pasting_marker_content>> const &erased,
                                 std::vector<std::pair<std::size_t, pasting_marker_content>> const &inserted,
                                 std::vector<std::pair<std::size_t, pasting_marker_content>> const &replaced) {
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

void ui_pasting_markers::_set_count(std::size_t const content_count) {
    auto const prev_element_count = this->_elements.size();

    if (prev_element_count < content_count) {
        this->_elements.reserve(
            common_utils::reserving_count(content_count, ui_pasting_markers_constants::reserving_interval));

        auto each = make_fast_each(content_count - prev_element_count);
        while (yas_each_next(each)) {
            auto element = ui_pasting_marker_element::make_shared(this->_project_lifetime_id, this->_node);
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
