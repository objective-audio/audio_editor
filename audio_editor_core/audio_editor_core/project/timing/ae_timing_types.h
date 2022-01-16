//
//  ae_timing_types.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_number_components.h>

#include <string>

namespace yas::ae {
enum class timing_fraction_kind {
    sample,
    milisecond,
    frame30,
};

enum class timing_unit_kind : uint32_t {
    fraction = 0,
    seconds = 1,
    minutes = 2,
    hours = 3,
};

struct timing_components {
    struct args {
        bool is_minus = false;
        uint8_t hours = 0;
        uint8_t minutes = 0;
        uint8_t seconds = 0;
        uint32_t fraction = 0;
    };

    timing_components(args const &);

    bool is_minus() const;
    uint8_t hours() const;
    uint8_t minutes() const;
    uint8_t seconds() const;
    uint32_t fraction() const;

    bool is_zero() const;
    timing_components abs() const;

    bool operator==(timing_components const &rhs) const;
    bool operator!=(timing_components const &rhs) const;
    bool operator<(timing_components const &) const;

   private:
    bool _is_minus = false;
    uint8_t _hours = 0;
    uint8_t _minutes = 0;
    uint8_t _seconds = 0;
    uint32_t _fraction = 0;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::timing_components const &);
}

std::ostream &operator<<(std::ostream &, yas::ae::timing_components const &);
