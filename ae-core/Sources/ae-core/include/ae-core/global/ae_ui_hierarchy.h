//
//  ae_ui_hierarchy.h
//

#pragma once

#include <ae-core/global/ae_hierarchy.h>

#include <ae-core/ui_base/ae_ui_base_lifetime.hpp>
#include <ae-core/ui_resource/ae_ui_resource_lifetime.hpp>

namespace yas::ae {
struct ui_hierarchy final {
    [[nodiscard]] static std::shared_ptr<ui_resource_lifetime> const &resource_lifetime_for_project_lifetime_id(
        project_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<ui_base_lifetime> const &base_lifetime_for_project_lifetime_id(
        project_lifetime_id const &);
};
}  // namespace yas::ae
