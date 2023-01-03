//
//  ae_ui_atlas.cpp
//

#include "ae_ui_atlas.hpp"
#include <ui/yas_ui_texture_element.h>

using namespace yas;
using namespace yas::ae;

ui_atlas::ui_atlas(std::shared_ptr<ui::texture> const &texture) : _texture(texture) {
    if (!texture) {
        return;
    }

    this->_white_filled_element = texture->add_draw_handler({2, 2}, [](CGContextRef const ctx) {
        CGContextSetRGBFillColor(ctx, 1, 1, 1, 1);
        CGContextFillRect(ctx, CGRectMake(0, 0, 2, 2));
    });
}

std::shared_ptr<ui::texture> const &ui_atlas::texture() const {
    return this->_texture;
}

observing::syncable ui_atlas::observe_white_filled_tex_coords(
    std::function<void(ui::uint_region const &)> &&handler) const {
    return this->_white_filled_element->observe_tex_coords(
        [handler = std::move(handler)](ui::uint_region const &tex_coords) {
            handler({.origin = {.x = tex_coords.origin.x + tex_coords.size.width / 2,
                                .y = tex_coords.origin.y + tex_coords.size.height / 2},
                     .size = ui::uint_size::zero()});
        });
}
