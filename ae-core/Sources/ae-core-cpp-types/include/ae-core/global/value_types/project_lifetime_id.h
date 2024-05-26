//
//  project_lifetime_id.h
//

#pragma once

#include <ae-core/global/value_types/project_guid.h>
#include <cpp-utils/identifier.h>

namespace yas::ae {
struct project_lifetime_id final {
    identifier instance;
    project_guid guid;

    bool operator==(project_lifetime_id const &) const;
    bool operator!=(project_lifetime_id const &) const;
    bool operator<(project_lifetime_id const &) const;
};
}  // namespace yas::ae
