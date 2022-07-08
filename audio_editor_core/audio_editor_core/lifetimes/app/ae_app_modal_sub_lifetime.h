//
//  ae_app_modal_sub_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_project_setup_dialog_lifetime.h>

#include <optional>
#include <variant>

namespace yas::ae {
using app_modal_sub_lifetime = std::variant<std::shared_ptr<project_setup_dialog_lifetime>>;
}

namespace yas {
template <typename T>
std::shared_ptr<T> const &get(std::optional<ae::app_modal_sub_lifetime> const &);
}
