//
//  project_settings_opener.hpp
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>

namespace yas::ae {
class project_settings_lifecycle;

struct project_settings_opener {
    project_settings_opener(project_lifetime_id const, project_settings_lifecycle *);

    [[nodiscard]] bool can_open() const;

    void open();

   private:
    project_lifetime_id const _lifetime_id;
    project_settings_lifecycle *const _lifecycle;

    project_settings_opener(project_settings_opener const &) = delete;
    project_settings_opener(project_settings_opener &&) = delete;
    project_settings_opener &operator=(project_settings_opener const &) = delete;
    project_settings_opener &operator=(project_settings_opener &&) = delete;
};
}  // namespace yas::ae
