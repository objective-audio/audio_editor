//
//  ae_ui_resource_lifecycle.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <ui/yas_ui_standard.h>

#include <map>
#include <string>

namespace yas::ae {
class ui_resource_lifetime;

struct ui_resource_lifecycle final {
    ui_resource_lifecycle();

    void add_lifetime(std::shared_ptr<ui::standard> const &, project_lifetime_id const &);
    void remove_lifetime_for_project_lifetime_id(project_lifetime_id const &);
    [[nodiscard]] std::shared_ptr<ui_resource_lifetime> const &lifetime_for_project_lifetime_id(
        project_lifetime_id const &) const;

   private:
    std::map<project_lifetime_id, std::shared_ptr<ui_resource_lifetime>> _lifetimes;

    ui_resource_lifecycle(ui_resource_lifecycle const &) = delete;
    ui_resource_lifecycle(ui_resource_lifecycle &&) = delete;
    ui_resource_lifecycle &operator=(ui_resource_lifecycle const &) = delete;
    ui_resource_lifecycle &operator=(ui_resource_lifecycle &&) = delete;
};
}  // namespace yas::ae
