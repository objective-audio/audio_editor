//
//  ae_time_presenter_types.cpp
//

#include "ae_time_presenter_types.h"

#include <iomanip>
#include <sstream>

using namespace yas;
using namespace yas::ae;

std::string time_components::sign_string() const {
    return this->is_minus ? "-" : "+";
}

std::string time_components::hours_string() const {
    std::stringstream stream;
    stream << std::setfill('0') << std::right << std::setw(2) << static_cast<uint16_t>(this->hours);
    return stream.str();
}

std::string time_components::minutes_string() const {
    std::stringstream stream;
    stream << std::setfill('0') << std::right << std::setw(2) << static_cast<uint16_t>(this->minutes);
    return stream.str();
}

std::string time_components::seconds_string() const {
    std::stringstream stream;
    stream << std::setfill('0') << std::right << std::setw(2) << static_cast<uint16_t>(this->seconds);
    return stream.str();
}

std::string time_components::fraction_string() const {
    std::stringstream stream;
    if (this->fraction_digits > 0) {
        stream << std::setfill('0') << std::right << std::setw(this->fraction_digits) << this->fraction;
    }
    return stream.str();
}
