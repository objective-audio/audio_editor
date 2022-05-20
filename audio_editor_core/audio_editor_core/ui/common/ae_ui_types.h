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
using triangle_index2d_rect = ui::index2d_rect;

struct line_index2d_rect final {
    static std::size_t constexpr vector_count = 8;

    ui::index2d_t v[vector_count];

    void set_all(ui::index2d_t const first);
};
}  // namespace yas::ae
