//
//  ae_ui_button.mm
//

#include "ae_ui_button.h"
#include <audio_editor_core/ae_ui_layout_utils.h>

using namespace yas;
using namespace yas::ae;

ui_button::ui_button(std::shared_ptr<ui::font_atlas> const &font_atlas)
    : _button(ui::button::make_shared({.origin = ui::point::zero(), .size = {60.0f, 44.0f}})),
      _strings(ui::strings::make_shared({.font_atlas = font_atlas, .alignment = ui::layout_alignment::mid})) {
    this->_button->rect_plane()->node()->mesh()->set_use_mesh_color(true);
    this->_button->rect_plane()->data()->set_rect_color(to_float4(ui::orange_color(), 1.0f), to_rect_index(0, false));
    this->_button->rect_plane()->data()->set_rect_color(to_float4(ui::cyan_color(), 1.0f), to_rect_index(0, true));

    this->node()->add_sub_node(this->_strings->rect_plane()->node());

    this->_button->layout_guide()
        ->observe([this](ui::region const &region) {
            auto const &font_atlas = this->_strings->font_atlas();
            float const text_height = font_atlas->ascent() + font_atlas->descent();
            this->_strings->rect_plane()->node()->set_position(region.center() +
                                                               ui::point{.x = 0.0f, .y = text_height * 0.5f});
        })
        .sync()
        ->add_to(this->_pool);
}

void ui_button::set_text(std::string const &text) {
    this->_strings->set_text(text);
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

std::shared_ptr<ui_button> ui_button::make_shared(std::shared_ptr<ui::font_atlas> const &font_atlas) {
    return std::shared_ptr<ui_button>(new ui_button{font_atlas});
}
