//
//  ae_ui_base_lifecycle.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_project_lifetime_id.h>

#include <memory>

namespace yas::ae {
class ui_base_lifetime;

struct ui_base_lifecycle final {
    ui_base_lifecycle(project_lifetime_id const &);

    project_lifetime_id const project_lifetime_id;

    void add_lifetime();
    std::shared_ptr<ui_base_lifetime> const &lifetime() const;

   private:
    std::shared_ptr<ui_base_lifetime> _lifetime;

    ui_base_lifecycle(ui_base_lifecycle const &) = delete;
    ui_base_lifecycle(ui_base_lifecycle &&) = delete;
    ui_base_lifecycle &operator=(ui_base_lifecycle const &) = delete;
    ui_base_lifecycle &operator=(ui_base_lifecycle &&) = delete;
};
}  // namespace yas::ae
