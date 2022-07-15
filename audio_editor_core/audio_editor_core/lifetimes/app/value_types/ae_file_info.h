//
//  ae_file_info.h
//

#pragma once

#include <cstdint>

namespace yas::ae {
struct file_info final {
    uint32_t sample_rate;
    uint32_t channel_count;
    uint64_t length;

    bool operator==(file_info const &rhs) const;
    bool operator!=(file_info const &rhs) const;
};
}  // namespace yas::ae
