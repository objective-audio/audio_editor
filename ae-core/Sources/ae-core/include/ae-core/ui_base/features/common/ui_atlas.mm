//
//  ui_atlas.cpp
//

#include "ui_atlas.hpp"
#include <ui/texture/yas_ui_texture_element.h>

using namespace yas;
using namespace yas::ae;

ui_atlas::ui_atlas(std::shared_ptr<ui::texture> const &texture)
    : _texture(texture), _tex_coords(observing::value::holder<ui::uint_region>::make_shared(ui::uint_region::zero())) {
    if (!texture) {
        return;
    }

    auto const element = texture->add_draw_handler({2, 2}, [](CGContextRef const ctx) {
        CGContextSetRGBFillColor(ctx, 1, 1, 1, 1);
        CGContextFillRect(ctx, CGRectMake(0, 0, 2, 2));
    });

    element
        ->observe_tex_coords([this](ui::uint_region const &tex_coords) {
            this->_tex_coords->set_value({.origin = {.x = tex_coords.origin.x + tex_coords.size.width / 2,
                                                     .y = tex_coords.origin.y + tex_coords.size.height / 2},
                                          .size = ui::uint_size::zero()});
        })
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::texture> const &ui_atlas::texture() const {
    return this->_texture;
}

ui::uint_region const &ui_atlas::white_filled_tex_coords() const {
    return this->_tex_coords->value();
}

observing::syncable ui_atlas::observe_white_filled_tex_coords(
    std::function<void(ui::uint_region const &)> &&handler) const {
    return this->_tex_coords->observe(std::move(handler));
}
