//
//  ae_context_menu_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_context_menu.h>

#include <memory>

namespace yas::ae {
struct context_menu_lifetime final {
    [[nodiscard]] static std::shared_ptr<context_menu_lifetime> make_shared(context_menu const &);

    context_menu_lifetime(context_menu const &);

    context_menu const context_menu;
};
}  // namespace yas::ae
