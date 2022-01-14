//
//  ae_timing_types.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <string>

namespace yas::ae {
enum class timing_fraction_kind {
    sample,
    milisecond,
    frame30,
};

struct timing_components {
    bool is_minus = false;
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;
    uint32_t fraction = 0;

    bool is_zero() const;
    timing_components abs() const;

    bool operator==(timing_components const &rhs) const;
    bool operator!=(timing_components const &rhs) const;
    bool operator<(timing_components const &) const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::timing_components const &);
}

std::ostream &operator<<(std::ostream &, yas::ae::timing_components const &);
