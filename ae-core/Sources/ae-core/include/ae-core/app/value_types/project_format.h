//
//  project_format.h
//

#pragma once

#include <cstdint>

namespace yas::ae {
struct project_format final {
    uint32_t sample_rate;
    uint32_t channel_count;

    bool operator==(project_format const &rhs) const;
    bool operator!=(project_format const &rhs) const;
};
}  // namespace yas::ae
