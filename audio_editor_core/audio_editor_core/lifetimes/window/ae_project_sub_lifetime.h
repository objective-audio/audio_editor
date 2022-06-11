//
//  ae_project_sub_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_project_lifetime.h>

#include <optional>
#include <variant>

namespace yas::ae {
using project_sub_lifetime = std::variant<std::shared_ptr<project_lifetime>>;
}

namespace yas {
template <typename T>
std::shared_ptr<T> const &get(std::optional<ae::project_sub_lifetime> const &);
}
