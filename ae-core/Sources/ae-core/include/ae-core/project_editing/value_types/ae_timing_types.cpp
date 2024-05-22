//
//  ae_timing_types.cpp
//

#include <ae-core/global/utils/ae_math.h>
#include <ae-core/project_editing/value_types/ae_timing_types.h>
#include <cpp-utils/fast_each.h>

#include <cassert>

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

std::size_t timing_components::fraction_unit_size() const {
    return this->_components.unit(to_index(timing_unit_kind::fraction)).size;
}

bool timing_components::is_zero() const {
    return this->_components.is_zero();
}

timing_components timing_components::abs() const {
    if (this->is_minus()) {
        return {{.is_minus = false,
                 .hours = static_cast<uint8_t>(this->value(timing_unit_kind::hours)),
                 .minutes = static_cast<uint8_t>(this->value(timing_unit_kind::minutes)),
                 .seconds = static_cast<uint8_t>(this->value(timing_unit_kind::seconds)),
                 .fraction = static_cast<uint32_t>(this->value(timing_unit_kind::fraction)),
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

timing_components timing_components::offset(offset_args &&args) {
    std::vector<number_components_unit> units{{.size = args.fraction_unit_size, .value = 0},
                                              {.size = 60, .value = 0},
                                              {.size = 60, .value = 0},
                                              {.size = 100, .value = 0}};

    uint32_t carry_over = 0;

    auto each = make_fast_each(units.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto &unit = units.at(idx);

        if (idx == args.unit_index) {
            carry_over = args.count;
        }

        if (carry_over > 0) {
            unit.value = carry_over % unit.size;
            carry_over = carry_over / unit.size;
        }
    }

    assert(carry_over == 0);

    return timing_components{{args.is_minus, std::move(units)}};
}

ae::timing_fraction_kind yas::rotate_next(ae::timing_fraction_kind const kind) {
    switch (kind) {
        case timing_fraction_kind::sample:
            return timing_fraction_kind::milisecond;
        case timing_fraction_kind::milisecond:
            return timing_fraction_kind::frame30;
        case timing_fraction_kind::frame30:
            return timing_fraction_kind::sample;
    }
}

std::size_t yas::to_index(timing_unit_kind const kind) {
    return static_cast<std::underlying_type_t<timing_unit_kind>>(kind);
}

ae::timing_unit_kind yas::rotate_next(ae::timing_unit_kind const kind) {
    switch (kind) {
        case ae::timing_unit_kind::fraction:
            return ae::timing_unit_kind::hours;
        case ae::timing_unit_kind::seconds:
            return ae::timing_unit_kind::fraction;
        case ae::timing_unit_kind::minutes:
            return ae::timing_unit_kind::seconds;
        case ae::timing_unit_kind::hours:
            return ae::timing_unit_kind::minutes;
    }
}

ae::timing_unit_kind yas::rotate_previous(ae::timing_unit_kind const kind) {
    switch (kind) {
        case ae::timing_unit_kind::fraction:
            return ae::timing_unit_kind::seconds;
        case ae::timing_unit_kind::seconds:
            return ae::timing_unit_kind::minutes;
        case ae::timing_unit_kind::minutes:
            return ae::timing_unit_kind::hours;
        case ae::timing_unit_kind::hours:
            return ae::timing_unit_kind::fraction;
    }
}

std::string yas::to_string(ae::timing_components const &components) {
    return "{is_minus:" + std::to_string(components.is_minus()) +
           ", hours:" + std::to_string(components.value(timing_unit_kind::hours)) +
           ", minutes:" + std::to_string(components.value(timing_unit_kind::minutes)) +
           ", seconds:" + std::to_string(components.value(timing_unit_kind::seconds)) +
           ", fraction:" + std::to_string(components.value(timing_unit_kind::fraction)) +
           ", fraction_unit_size:" + std::to_string(components.fraction_unit_size()) + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::timing_components const &value) {
    os << to_string(value);
    return os;
}
