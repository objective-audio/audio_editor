//
//  ae_app_dialog_sub_level.h
//

#pragma once

#include <audio_editor_core/ae_project_setup_dialog_level.h>

#include <optional>
#include <variant>

namespace yas::ae {
using app_dialog_sub_level = std::variant<std::shared_ptr<project_setup_dialog_level>>;
}

namespace yas {
template <typename T>
std::shared_ptr<T> const &get_level(std::optional<ae::app_dialog_sub_level> const &);
}
