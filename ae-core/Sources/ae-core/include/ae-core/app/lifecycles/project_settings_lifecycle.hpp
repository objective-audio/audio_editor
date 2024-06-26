//
//  project_settings_lifecycle.hpp
//

#pragma once

#include <ae-core/app/lifecycles/project_settings_lifecycle_event.h>
#include <ae-core/global/value_types/project_lifetime_id.h>

#include <observing/umbrella.hpp>

namespace yas::ae {
class project_settings_lifetime;

struct project_settings_lifecycle final {
    project_settings_lifecycle();

    void add_lifetime(project_lifetime_id const &);
    void remove_lifetime(project_lifetime_id const &);

    [[nodiscard]] std::shared_ptr<project_settings_lifetime> const &lifetime_for_id(project_lifetime_id const &) const;

    [[nodiscard]] observing::syncable observe_event(std::function<void(project_settings_lifecycle_event const &)> &&);

   private:
    using lifetimes_t = observing::map::holder<project_lifetime_id, std::shared_ptr<project_settings_lifetime>>;
    std::shared_ptr<lifetimes_t> const _lifetimes;

    project_settings_lifecycle(project_settings_lifecycle const &) = delete;
    project_settings_lifecycle(project_settings_lifecycle &&) = delete;
    project_settings_lifecycle &operator=(project_settings_lifecycle const &) = delete;
    project_settings_lifecycle &operator=(project_settings_lifecycle &&) = delete;
};
}  // namespace yas::ae
