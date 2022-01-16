//
//  ae_timing_types.cpp
//

#include <audio_editor_core/ae_math.h>
#include <audio_editor_core/ae_timing_types.h>

using namespace yas;
using namespace yas::ae;

timing_components::timing_components(args const &args)
    : _is_minus(args.is_minus),
      _hours(args.hours),
      _minutes(args.minutes),
      _seconds(args.seconds),
      _fraction(args.fraction) {
}

bool timing_components::is_minus() const {
    return this->_is_minus;
}

uint8_t timing_components::hours() const {
    return this->_hours;
}

uint8_t timing_components::minutes() const {
    return this->_minutes;
}

uint8_t timing_components::seconds() const {
    return this->_seconds;
}

uint32_t timing_components::fraction() const {
    return this->_fraction;
}

bool timing_components::is_zero() const {
    return this->_hours == 0 && this->_minutes == 0 && this->_seconds == 0 && this->_fraction == 0;
}

timing_components timing_components::abs() const {
    if (this->is_minus()) {
        return {{.is_minus = false,
                 .hours = this->_hours,
                 .minutes = this->_minutes,
                 .seconds = this->_seconds,
                 .fraction = this->_fraction}};
    } else {
        return *this;
    }
}

bool timing_components::operator==(timing_components const &rhs) const {
    if (this->is_zero() && rhs.is_zero()) {
        return true;
    } else {
        return this->is_minus() == rhs.is_minus() && this->_hours == rhs._hours && this->_minutes == rhs._minutes &&
               this->_seconds == rhs._seconds && this->_fraction == rhs._fraction;
    }
}

bool timing_components::operator!=(timing_components const &rhs) const {
    return !(*this == rhs);
}

bool timing_components::operator<(timing_components const &rhs) const {
    if (this->is_minus() && !rhs.is_minus()) {
        return true;
    } else if (!this->is_minus() && rhs.is_minus()) {
        return false;
    }

    bool is_minus = this->is_minus();

    if (this->_hours < rhs._hours) {
        return !is_minus;
    } else if (this->_hours > rhs._hours) {
        return is_minus;
    }

    if (this->_minutes < rhs._minutes) {
        return !is_minus;
    } else if (this->_minutes > rhs._minutes) {
        return is_minus;
    }

    if (this->_seconds < rhs._seconds) {
        return !is_minus;
    } else if (this->_seconds > rhs._seconds) {
        return is_minus;
    }

    if (this->_fraction < rhs._fraction) {
        return !is_minus;
    } else if (this->_fraction > rhs._fraction) {
        return is_minus;
    }

    return false;
}

std::string yas::to_string(ae::timing_components const &components) {
    return "{is_minus:" + std::to_string(components.is_minus()) + ", hours:" + std::to_string(components.hours()) +
           ", minutes:" + std::to_string(components.minutes()) + ", seconds:" + std::to_string(components.seconds()) +
           ", fraction:" + std::to_string(components.fraction()) + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::timing_components const &value) {
    os << to_string(value);
    return os;
}
