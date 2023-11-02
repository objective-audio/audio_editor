#include "ae_ui_track_name.hpp"
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::unique_ptr<ui_track_name> ui_track_name::make_unique(project_lifetime_id const &lifetime_id,
                                                          ui::node *parent_node) {
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(lifetime_id);

    return std::unique_ptr<ui_track_name>(new ui_track_name{resource_lifetime->normal_font_atlas, parent_node});
}

ui_track_name::ui_track_name(std::shared_ptr<ui::font_atlas> const &font_atlas, ui::node *parent_node)
    : _name_strings(ui::strings::make_shared(
          {.frame = ui::region{.origin = ui::point::zero(),
                               .size = {0.0f, -static_cast<float>(font_atlas->ascent() + font_atlas->descent())}}},
          font_atlas)) {
    parent_node->add_sub_node(this->node());
}

std::shared_ptr<ui::node> const &ui_track_name::node() const {
    return this->_name_strings->rect_plane()->node();
}

void ui_track_name::set_content(track_content const &content) {
    this->_content = std::nullopt;

    this->update_content(content);
}

void ui_track_name::update_content(track_content const &content) {
    if (this->_content.has_value() && this->_content.value().identifier != content.identifier) {
        assertion_failure_if_not_test();
    }

    this->_content = content;

    auto const &node = this->node();
    node->set_is_enabled(true);
    node->set_y(this->_content->y());

    this->_name_strings->set_text(content.name());
}

void ui_track_name::reset_content() {
    this->_cancellable = nullptr;
    this->_content = std::nullopt;
    this->node()->set_is_enabled(false);
}

void ui_track_name::update_color(ui::color const &selected_color, ui::color const &normal_color) {
    if (this->_content.has_value()) {
        bool const is_selected = this->_content.value().is_selected;
        auto const &text_color = is_selected ? selected_color : normal_color;

        this->node()->set_color(text_color);
    }
}

void ui_track_name::finalize() {
    this->node()->remove_from_super_node();
}
