//
//  ae_file_loader_types.h
//

#pragma once

#include <cstdint>

namespace yas::ae {
struct file_info final {
    double sample_rate;
    uint32_t channel_count;
    uint64_t length;

    bool operator==(file_info const &rhs) const {
        return this->sample_rate == rhs.sample_rate && this->channel_count == rhs.channel_count &&
               this->length == rhs.length;
    }

    bool operator!=(file_info const &rhs) const {
        return !(*this == rhs);
    }
};
}  // namespace yas::ae
