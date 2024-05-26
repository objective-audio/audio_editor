//
//  project_closer.h
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>

namespace yas::ae {
class project_lifecycle;
class project_settings_lifecycle;

struct project_closer final {
    project_closer(project_lifetime_id const &, project_lifecycle *, project_settings_lifecycle *);

    [[nodiscard]] bool can_close() const;
    void close_if_needed();

   private:
    project_lifetime_id const _lifetime_id;
    project_lifecycle *_project_lifecycle;
    project_settings_lifecycle *_project_settings_lifecycle;

    project_closer(project_closer const &) = delete;
    project_closer(project_closer &&) = delete;
    project_closer &operator=(project_closer const &) = delete;
    project_closer &operator=(project_closer &&) = delete;
};
}  // namespace yas::ae
