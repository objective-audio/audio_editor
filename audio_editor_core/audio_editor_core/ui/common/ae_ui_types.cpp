//
//  ae_ui_types.cpp
//

#include "ae_ui_types.h"

using namespace yas;
using namespace yas::ae;

void ae::frame_index2d_rect::set_all(ui::index2d_t const first) {
    this->v[0] = this->v[7] = first;
    this->v[1] = this->v[2] = first + 1;
    this->v[3] = this->v[4] = first + 3;
    this->v[5] = this->v[6] = first + 2;
}

void ae::vertex2d_line::set_position(ui::point const first, ui::point const second) {
    this->v[0].position.x = first.x;
    this->v[0].position.y = first.y;
    this->v[1].position.x = second.x;
    this->v[1].position.y = second.y;
}

void ae::vertex2d_line::reset_position() {
    this->v[0].position.x = this->v[0].position.y = this->v[1].position.x = this->v[1].position.y = 0.0f;
}

void ae::vertex2d_line::set_color(simd::float4 const &color) {
    this->v[0].color = color;
    this->v[1].color = color;
}

void ae::vertex2d_line::set_color(ui::color const &color) {
    this->set_color(color.v);
}

void ae::vertex2d_line::set_tex_coord(ui::uint_point const &point) {
    this->v[0].tex_coord.x = this->v[1].tex_coord.x = point.x;
    this->v[0].tex_coord.y = this->v[1].tex_coord.y = point.y;
}

void ae::index2d_line::set_all(ui::index2d_t const first) {
    this->v[0] = first;
    this->v[1] = first + 1;
}
