//
//  project_guid.h
//

#pragma once

#include <string>

namespace yas::ae {
struct project_guid final {
    std::string raw_value;

    bool operator==(project_guid const &rhs) const;
    bool operator!=(project_guid const &rhs) const;
    bool operator<(project_guid const &rhs) const;
};
}  // namespace yas::ae
