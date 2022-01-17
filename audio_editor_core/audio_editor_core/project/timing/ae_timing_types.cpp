//
//  ae_timing_types.cpp
//

#include <audio_editor_core/ae_math.h>
#include <audio_editor_core/ae_timing_types.h>

using namespace yas;
using namespace yas::ae;

timing_components::timing_components(number_components const &components) : _components(components) {
    if (components.size() != 4) {
        throw std::invalid_argument("units count is not equal 4.");
    }

    if (components.unit(to_index(timing_unit_kind::seconds)).size != 60) {
        throw std::invalid_argument("seconds unit count is not equal 60.");
    }

    if (components.unit(to_index(timing_unit_kind::minutes)).size != 60) {
        throw std::invalid_argument("minutes unit count is not equal 60.");
    }

    if (components.unit(to_index(timing_unit_kind::hours)).size != 100) {
        throw std::invalid_argument("hours unit count is not equal 100.");
    }
}

timing_components::timing_components(args const &args)
    : timing_components({args.is_minus,
                         {{.size = args.fraction_unit_size, .value = args.fraction},
                          {.size = 60, .value = args.seconds},
                          {.size = 60, .value = args.minutes},
                          {.size = 100, .value = args.hours}}}) {
}

number_components const &timing_components::raw_components() const {
    return this->_components;
}

bool timing_components::is_minus() const {
    return this->_components.is_minus();
}

uint32_t timing_components::value(timing_unit_kind const kind) const {
    return this->_components.unit(to_index(kind)).value;
}

uint8_t timing_components::hours() const {
    return this->value(timing_unit_kind::hours);
}

uint8_t timing_components::minutes() const {
    return this->value(timing_unit_kind::minutes);
}

uint8_t timing_components::seconds() const {
    return this->value(timing_unit_kind::seconds);
}

uint32_t timing_components::fraction() const {
    return this->value(timing_unit_kind::fraction);
}

std::size_t timing_components::fraction_unit_size() const {
    return this->_components.unit(to_index(timing_unit_kind::fraction)).size;
}

bool timing_components::is_zero() const {
    return this->_components.is_zero();
}

timing_components timing_components::abs() const {
    if (this->is_minus()) {
        return {{.is_minus = false,
                 .hours = this->hours(),
                 .minutes = this->minutes(),
                 .seconds = this->seconds(),
                 .fraction = this->fraction(),
                 .fraction_unit_size = this->fraction_unit_size()}};
    } else {
        return *this;
    }
}

bool timing_components::operator==(timing_components const &rhs) const {
    return this->_components == rhs._components;
}

bool timing_components::operator!=(timing_components const &rhs) const {
    return !(*this == rhs);
}

bool timing_components::operator<(timing_components const &rhs) const {
    return this->_components < rhs._components;
}

timing_components timing_components::adding(timing_components const &rhs) const {
    return {this->_components.adding(rhs._components)};
}

std::size_t yas::to_index(timing_unit_kind const kind) {
    return static_cast<std::underlying_type_t<timing_unit_kind>>(kind);
}

std::string yas::to_string(ae::timing_components const &components) {
    return "{is_minus:" + std::to_string(components.is_minus()) + ", hours:" + std::to_string(components.hours()) +
           ", minutes:" + std::to_string(components.minutes()) + ", seconds:" + std::to_string(components.seconds()) +
           ", fraction:" + std::to_string(components.fraction()) + "" +
           std::to_string(components.fraction_unit_size()) + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::timing_components const &value) {
    os << to_string(value);
    return os;
}
