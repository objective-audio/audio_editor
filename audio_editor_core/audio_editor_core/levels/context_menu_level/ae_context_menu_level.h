//
//  ae_context_menu_level.hpp
//  audio_editor_core
//
//  Created by Yuki Yasoshima on 2022/05/21.
//

#pragma once

#include <audio_editor_core/ae_context_menu.h>

#include <memory>

namespace yas::ae {
struct context_menu_level final {
    [[nodiscard]] static std::shared_ptr<context_menu_level> make_shared(context_menu const &);

    context_menu_level(context_menu const &);

    context_menu const context_menu;
};
}  // namespace yas::ae
