//
//  ae_ui_types.h
//  audio_editor_core
//
//  Created by Yuki Yasoshima on 2022/01/27.
//

#pragma once

#include <ui/yas_ui_types.h>

namespace yas::ae {
using vertex2d_rect = ui::vertex2d_rect;
using fill_index2d_rect = ui::index2d_rect;

struct frame_index2d_rect final {
    static std::size_t constexpr vector_count = 8;

    ui::index2d_t v[vector_count];

    void set_all(ui::index2d_t const first);
};

struct vertex2d_line final {
    static std::size_t constexpr vector_count = 2;

    ui::vertex2d_t v[vector_count];

    void set_position(ui::point const first, ui::point const second);
    void reset_position();
    void set_color(simd::float4 const &);
    void set_color(ui::color const &);
    void set_tex_coord(ui::uint_point const &);
};

struct index2d_line final {
    static std::size_t constexpr vector_count = 2;

    ui::index2d_t v[vector_count];

    void set_all(ui::index2d_t const first);
};
}  // namespace yas::ae
