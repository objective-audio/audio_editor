//
//  ae_project_sub_level.h
//

#pragma once

#include <audio_editor_core/ae_dialog_level.h>
#include <audio_editor_core/ae_sheet_level.h>
#include <audio_editor_core/ae_time_editor_level.h>

#include <optional>
#include <variant>

namespace yas::ae {
using project_sub_level =
    std::variant<std::shared_ptr<time_editor_level>, std::shared_ptr<sheet_level>, std::shared_ptr<dialog_level>>;
}  // namespace yas::ae

namespace yas {
std::shared_ptr<ae::time_editor_level> const &get_time_editor_level(std::optional<ae::project_sub_level> const &);
std::shared_ptr<ae::sheet_level> const &get_sheet_level(std::optional<ae::project_sub_level> const &);
std::shared_ptr<ae::dialog_level> const &get_dialog_level(std::optional<ae::project_sub_level> const &);
}  // namespace yas
