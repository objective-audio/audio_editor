//
//  ae_context_menu_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_context_menu.h>
#include <audio_editor_core/ae_project_sub_lifetime_id.h>

#include <memory>

namespace yas::ae {
struct context_menu_lifetime final {
    [[nodiscard]] static std::shared_ptr<context_menu_lifetime> make_shared(project_sub_lifetime_id const &,
                                                                            context_menu const &);

    context_menu_lifetime(project_sub_lifetime_id const &, context_menu const &);

    project_sub_lifetime_id const lifetime_id;
    context_menu const context_menu;
};
}  // namespace yas::ae
