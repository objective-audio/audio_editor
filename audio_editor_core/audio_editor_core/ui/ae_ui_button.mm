//
//  ae_ui_button.mm
//

#include "ae_ui_button.h"
#include <audio_editor_core/ae_ui_button_utils.h>
#include <audio_editor_core/ae_ui_layout_utils.h>

using namespace yas;
using namespace yas::ae;

ui_button::ui_button(std::shared_ptr<ui::font_atlas> const &font_atlas, std::shared_ptr<ui::standard> const &standard)
    : _button(ui::button::make_shared(ui::region::zero(), 2, standard->event_manager(), standard->detector())),
      _strings(ui::strings::make_shared({.alignment = ui::layout_alignment::mid}, font_atlas)) {
    this->_button->rect_plane()->node()->mesh()->set_use_mesh_color(true);

    auto const disabled_state_idx = ui_button_utils::to_state_idx(ui_button_state::disabled);
    this->_button->rect_plane()->data()->set_rect_color(to_float4(ui::color{.v = 0.1f}, 1.0f),
                                                        to_rect_index(disabled_state_idx, false));
    this->_button->rect_plane()->data()->set_rect_color(to_float4(ui::color{.v = 0.1f}, 1.0f),
                                                        to_rect_index(disabled_state_idx, true));

    auto const enabled_state_idx = ui_button_utils::to_state_idx(ui_button_state::enabled);
    this->_button->rect_plane()->data()->set_rect_color(to_float4(ui::color{.v = 0.4f}, 1.0f),
                                                        to_rect_index(enabled_state_idx, false));
    this->_button->rect_plane()->data()->set_rect_color(to_float4(ui::color{.v = 0.5f}, 1.0f),
                                                        to_rect_index(enabled_state_idx, true));

    this->node()->add_sub_node(this->_strings->rect_plane()->node());

    auto const center = std::make_shared<ui::point>();
    auto const height = std::make_shared<float>();
    auto const layout_strings = [center, height, this] {
        ui::point const point{center->x, center->y + *height * 0.5f};
        this->_strings->rect_plane()->node()->set_position(point);
    };

    this->_button->layout_guide()
        ->observe([center, layout_strings](ui::region const &region) {
            *center = region.center();
            layout_strings();
        })
        .sync()
        ->add_to(this->_pool);

    this->_strings->actual_layout_source()
        ->layout_vertical_range_source()
        ->layout_length_value_source()
        ->observe_layout_value([height, layout_strings](float const &value) {
            *height = value;
            layout_strings();
        })
        .sync()
        ->add_to(this->_pool);
}

void ui_button::set_text(std::string const &text) {
    this->_strings->set_text(text);
}

void ui_button::set_enabled(bool const enabled) {
    this->_button->rect_plane()->node()->collider()->set_enabled(enabled);
    auto const state = enabled ? ui_button_state::enabled : ui_button_state::disabled;
    this->_button->set_state_index(ui_button_utils::to_state_idx(state));
}

std::shared_ptr<ui::node> const &ui_button::node() const {
    return this->_button->rect_plane()->node();
}

observing::endable ui_button::observe_tapped(std::function<void(void)> &&handler) {
    return this->_button->observe([handler = std::move(handler)](auto const &context) {
        if (context.method == ui::button::method::ended) {
            handler();
        }
    });
}

std::shared_ptr<ui::layout_region_guide> const &ui_button::layout_guide() const {
    return this->_button->layout_guide();
}

std::shared_ptr<ui_button> ui_button::make_shared(std::shared_ptr<ui::font_atlas> const &font_atlas,
                                                  std::shared_ptr<ui::standard> const &standard) {
    return std::shared_ptr<ui_button>(new ui_button{font_atlas, standard});
}
