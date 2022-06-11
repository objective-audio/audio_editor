//
//  ae_project_modal_sub_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_context_menu_lifetime.h>
#include <audio_editor_core/ae_dialog_lifetime.h>
#include <audio_editor_core/ae_sheet_lifetime.h>
#include <audio_editor_core/ae_time_editor_lifetime.h>

#include <optional>
#include <variant>

namespace yas::ae {
using project_modal_sub_lifetime =
    std::variant<std::shared_ptr<time_editor_lifetime>, std::shared_ptr<sheet_lifetime>,
                 std::shared_ptr<dialog_lifetime>, std::shared_ptr<context_menu_lifetime>>;
}  // namespace yas::ae

namespace yas {
template <typename T>
std::shared_ptr<T> const &get(std::optional<ae::project_modal_sub_lifetime> const &);
}  // namespace yas
