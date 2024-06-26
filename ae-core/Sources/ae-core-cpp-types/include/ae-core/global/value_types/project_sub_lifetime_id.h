//
//  project_sub_lifetime_id.h
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>

namespace yas::ae {
struct project_sub_lifetime_id final {
    identifier instance;
    project_lifetime_id project;

    bool operator==(project_sub_lifetime_id const &) const;
    bool operator!=(project_sub_lifetime_id const &) const;
    bool operator<(project_sub_lifetime_id const &) const;
};
}  // namespace yas::ae
