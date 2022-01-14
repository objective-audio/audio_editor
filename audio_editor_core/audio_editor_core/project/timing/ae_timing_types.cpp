//
//  ae_timing_types.cpp
//

#include <audio_editor_core/ae_math.h>
#include <audio_editor_core/ae_timing_types.h>

using namespace yas;
using namespace yas::ae;

bool timing_components::is_zero() const {
    return this->hours == 0 && this->minutes == 0 && this->seconds == 0 && this->fraction == 0;
}

timing_components timing_components::abs() const {
    if (this->is_minus) {
        auto abs = *this;
        abs.is_minus = false;
        return abs;
    } else {
        return *this;
    }
}

bool timing_components::operator==(timing_components const &rhs) const {
    if (this->is_zero() && rhs.is_zero()) {
        return true;
    } else {
        return this->is_minus == rhs.is_minus && this->hours == rhs.hours && this->minutes == rhs.minutes &&
               this->seconds == rhs.seconds && this->fraction == rhs.fraction;
    }
}

bool timing_components::operator!=(timing_components const &rhs) const {
    return !(*this == rhs);
}

bool timing_components::operator<(timing_components const &rhs) const {
    if (this->is_minus && !rhs.is_minus) {
        return true;
    } else if (!this->is_minus && rhs.is_minus) {
        return false;
    }

    bool is_minus = this->is_minus;

    if (this->hours < rhs.hours) {
        return !is_minus;
    } else if (this->hours > rhs.hours) {
        return is_minus;
    }

    if (this->minutes < rhs.minutes) {
        return !is_minus;
    } else if (this->minutes > rhs.minutes) {
        return is_minus;
    }

    if (this->seconds < rhs.seconds) {
        return !is_minus;
    } else if (this->seconds > rhs.seconds) {
        return is_minus;
    }

    if (this->fraction < rhs.fraction) {
        return !is_minus;
    } else if (this->fraction > rhs.fraction) {
        return is_minus;
    }

    return false;
}

std::string yas::to_string(ae::timing_components const &components) {
    return "{is_minus:" + std::to_string(components.is_minus) + ", hours:" + std::to_string(components.hours) +
           ", minutes:" + std::to_string(components.minutes) + ", seconds:" + std::to_string(components.seconds) +
           ", fraction:" + std::to_string(components.fraction) + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::timing_components const &value) {
    os << to_string(value);
    return os;
}
