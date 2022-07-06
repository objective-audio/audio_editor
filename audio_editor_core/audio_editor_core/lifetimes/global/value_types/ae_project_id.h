//
//  ae_project_id.h
//

#pragma once

#include <string>

namespace yas::ae {
struct project_id final {
    std::string raw_value;

    bool operator==(project_id const &rhs) const;
    bool operator!=(project_id const &rhs) const;
    bool operator<(project_id const &rhs) const;
};
}  // namespace yas::ae
