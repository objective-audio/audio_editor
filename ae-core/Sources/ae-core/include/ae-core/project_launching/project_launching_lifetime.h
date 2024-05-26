//
//  project_launching_lifetime.h
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>

#include <memory>

namespace yas::ae {
class project_launcher;
class project_lifetime;

struct project_launching_lifetime final {
    [[nodiscard]] static std::shared_ptr<project_launching_lifetime> make_shared(project_lifetime_id const &);

    project_launching_lifetime(project_lifetime *);

    project_lifetime_id const project_lifetime_id;

    std::shared_ptr<project_launcher> const launcher;

   private:
    project_launching_lifetime(project_launching_lifetime const &) = delete;
    project_launching_lifetime(project_launching_lifetime &&) = delete;
    project_launching_lifetime &operator=(project_launching_lifetime const &) = delete;
    project_launching_lifetime &operator=(project_launching_lifetime &&) = delete;
};
}  // namespace yas::ae
