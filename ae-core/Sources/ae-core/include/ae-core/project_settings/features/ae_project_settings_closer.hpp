//
//  ae_project_settings_closer.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_project_lifetime_id.h>

namespace yas::ae {
class project_settings_lifecycle;

struct project_settings_closer final {
    project_settings_closer(project_lifetime_id const &, project_settings_lifecycle *);

    [[nodiscard]] bool can_close() const;
    void close_if_needed();

   private:
    project_lifetime_id const _lifetime_id;
    project_settings_lifecycle *_lifecycle;

    project_settings_closer(project_settings_closer const &) = delete;
    project_settings_closer(project_settings_closer &&) = delete;
    project_settings_closer &operator=(project_settings_closer const &) = delete;
    project_settings_closer &operator=(project_settings_closer &&) = delete;
};
}  // namespace yas::ae
