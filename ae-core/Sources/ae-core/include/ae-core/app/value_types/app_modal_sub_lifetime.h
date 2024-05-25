//
//  app_modal_sub_lifetime.h
//

#pragma once

#include <ae-core/project_format_dialog/project_setup_dialog_lifetime.h>

#include <optional>
#include <variant>

namespace yas::ae {
using app_modal_sub_lifetime = std::variant<std::shared_ptr<project_setup_dialog_lifetime>>;

enum class app_modal_sub_lifetime_kind {
    none,
    project_setup_dialog,
};
}  // namespace yas::ae

namespace yas {
template <typename T>
std::shared_ptr<T> const &get(std::optional<ae::app_modal_sub_lifetime> const &);

ae::app_modal_sub_lifetime_kind to_kind(std::optional<ae::app_modal_sub_lifetime> const &);
}  // namespace yas
