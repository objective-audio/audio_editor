//
//  ae_project_sub_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_project_launch_lifetime.h>
#include <audio_editor_core/ae_project_lifetime.h>

#include <optional>
#include <variant>

namespace yas::ae {
using project_sub_lifetime = std::variant<std::shared_ptr<project_launch_lifetime>, std::shared_ptr<project_lifetime>>;

enum class project_sub_lifetime_kind {
    none,
    launch,
    project,
};
}  // namespace yas::ae

namespace yas {
template <typename T>
std::shared_ptr<T> const &get(std::optional<ae::project_sub_lifetime> const &);

ae::project_sub_lifetime_kind to_kind(std::optional<ae::project_sub_lifetime> const &);
}  // namespace yas
