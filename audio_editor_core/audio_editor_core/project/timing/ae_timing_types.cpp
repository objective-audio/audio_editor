//
//  ae_timing_types.cpp
//

#include <audio_editor_core/ae_math.h>
#include <audio_editor_core/ae_timing_types.h>

using namespace yas;
using namespace yas::ae;

timing_components::timing_components(args const &args)
    : _components(args.is_minus, {{.count = args.fraction_unit_count, .value = args.fraction},
                                  {.count = 60, .value = args.seconds},
                                  {.count = 60, .value = args.minutes},
                                  {.value = args.hours}}) {
}

bool timing_components::is_minus() const {
    return this->_components.is_minus();
}

uint8_t timing_components::hours() const {
    return this->_components.unit_value(to_index(timing_unit_kind::hours));
}

uint8_t timing_components::minutes() const {
    return this->_components.unit_value(to_index(timing_unit_kind::minutes));
}

uint8_t timing_components::seconds() const {
    return this->_components.unit_value(to_index(timing_unit_kind::seconds));
}

uint32_t timing_components::fraction() const {
    return this->_components.unit_value(to_index(timing_unit_kind::fraction));
}

uint32_t timing_components::fraction_unit_count() const {
    return this->_components.unit_count(to_index(timing_unit_kind::fraction));
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
                 .fraction_unit_count = this->fraction_unit_count()}};
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

std::size_t yas::to_index(timing_unit_kind const kind) {
    return static_cast<std::underlying_type_t<timing_unit_kind>>(kind);
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
