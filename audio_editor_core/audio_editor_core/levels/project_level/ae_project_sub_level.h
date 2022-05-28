//
//  ae_project_sub_level.h
//

#pragma once

#include <audio_editor_core/ae_context_menu_level.h>
#include <audio_editor_core/ae_dialog_level.h>
#include <audio_editor_core/ae_sheet_level.h>
#include <audio_editor_core/ae_time_editor_level.h>

#include <optional>
#include <variant>

namespace yas::ae {
using project_sub_level = std::variant<std::shared_ptr<time_editor_level>, std::shared_ptr<sheet_level>,
                                       std::shared_ptr<dialog_level>, std::shared_ptr<context_menu_level>>;
}  // namespace yas::ae

namespace yas {
template <typename T>
std::shared_ptr<T> const &get_level(std::optional<ae::project_sub_level> const &);
}  // namespace yas
