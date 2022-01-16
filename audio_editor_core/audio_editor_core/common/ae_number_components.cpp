//
//  ae_number_components.cpp
//

#include "ae_number_components.h"

#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

bool number_components::unit::operator==(unit const &rhs) const {
    return this->value == rhs.value && this->size == rhs.size;
}

bool number_components::unit::operator!=(unit const &rhs) const {
    return !(*this == rhs);
}

number_components::number_components(bool const is_minus, std::vector<unit> &&units)
    : _is_minus(is_minus), _units(std::move(units)) {
    if (this->_units.size() == 0) {
        throw std::invalid_argument("units is empty.");
    }

    std::size_t const last_idx = this->_units.size() - 1;

    auto each = make_fast_each(this->_units.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &unit = this->_units.at(idx);
        if (idx < last_idx || unit.size > 0) {
            if (unit.size == 0) {
                throw std::invalid_argument("count is zero.");
            } else if (unit.value >= unit.size) {
                throw std::invalid_argument("value exceeds count.");
            }
        }
    }
}

void number_components::set_is_minus(bool const is_minus) {
    this->_is_minus = is_minus;
}

bool number_components::is_minus() const {
    return this->_is_minus;
}

std::size_t number_components::size() const {
    return this->_units.size();
}

void number_components::set_unit_value(uint32_t const value, std::size_t const idx) {
    auto &unit = this->_units.at(idx);
    if (unit.size == 0 || value < unit.size) {
        unit.value = value;
    } else {
        throw std::overflow_error("value exceeds count.");
    }
}

uint32_t number_components::unit_value(std::size_t const idx) const {
    return this->_units.at(idx).value;
}

uint32_t number_components::unit_size(std::size_t const idx) const {
    return this->_units.at(idx).size;
}

bool number_components::is_zero() const {
    for (auto const &unit : this->_units) {
        if (unit.value != 0) {
            return false;
        }
    }
    return true;
}

number_components number_components::abs() const {
    if (this->_is_minus) {
        auto components = *this;
        components.set_is_minus(false);
        return components;
    } else {
        return *this;
    }
}

bool number_components::is_equal_structure(number_components const &rhs) const {
    if (this->_units.size() != rhs._units.size()) {
        return false;
    }

    auto each = make_fast_each(this->_units.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        if (this->_units.at(idx).size != rhs._units.at(idx).size) {
            return false;
        }
    }

    return true;
}

bool number_components::operator==(number_components const &rhs) const {
    if (this->_units == rhs._units) {
        if (this->_is_minus == rhs._is_minus) {
            return true;
        } else {
            return this->is_zero() && rhs.is_zero();
        }
    } else {
        return false;
    }
}

bool number_components::operator!=(number_components const &rhs) const {
    return !(*this == rhs);
}

bool number_components::operator<(number_components const &rhs) const {
    // 構造が違っていたらfalse
    if (!this->is_equal_structure(rhs)) {
        return false;
    }

    // 等しければfalse
    if (*this == rhs) {
        return false;
    }

    bool const is_lhs_zero = this->is_zero();
    bool const is_rhs_zero = rhs.is_zero();

    // すでに両方ゼロの可能性はないので、片方がゼロの場合は決まる
    if (is_lhs_zero) {
        return !rhs._is_minus;
    } else if (is_rhs_zero) {
        return this->_is_minus;
    }

    // 符号が違っていたら決まる
    if (this->_is_minus && !rhs._is_minus) {
        return true;
    } else if (!this->_is_minus && rhs._is_minus) {
        return false;
    }

    // 同じ符号なら値を比較する
    auto each = make_fast_each(this->_units.size());
    while (yas_each_next(each)) {
        auto const idx = this->_units.size() - yas_each_index(each) - 1;
        auto const &lhs_value = this->_units.at(idx).value;
        auto const &rhs_value = rhs._units.at(idx).value;

        if (lhs_value < rhs_value) {
            return !this->_is_minus;
        } else if (lhs_value > rhs_value) {
            return this->_is_minus;
        }
    }

    return false;
}

number_components number_components::adding(number_components const &rhs) const {
    if (!this->is_equal_structure(rhs)) {
        throw std::invalid_argument("not equal structure.");
    }

    if (this->is_zero()) {
        return rhs;
    } else if (rhs.is_zero()) {
        return *this;
    }

    if (this->_is_minus == rhs._is_minus) {
        std::vector<unit> units;
        uint64_t carry_over = 0;

        auto each = make_fast_each(this->_units.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &lhs_unit = this->_units.at(idx);
            uint64_t const value =
                static_cast<uint64_t>(lhs_unit.value) + static_cast<uint64_t>(rhs._units.at(idx).value) + carry_over;

            uint32_t const unit_value = static_cast<uint32_t>((lhs_unit.size > 0) ? (value % lhs_unit.size) : value);
            units.emplace_back(unit{.size = lhs_unit.size, .value = unit_value});

            carry_over = (lhs_unit.size > 0) ? value / lhs_unit.size : 0;
        }

        return number_components{this->_is_minus, std::move(units)};
    } else {
        auto const abs_lhs = this->abs();
        auto const abs_rhs = rhs.abs();

        bool const is_lhs_smaller = abs_lhs < abs_rhs;
        auto const &smaller = is_lhs_smaller ? abs_lhs : abs_rhs;
        auto const &larger = is_lhs_smaller ? abs_rhs : abs_lhs;

        std::vector<unit> units;
        int64_t carry_over = 0;

        auto each = make_fast_each(this->_units.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &larger_unit = larger._units.at(idx);

            int64_t const value = static_cast<int64_t>(larger_unit.value) -
                                  static_cast<int64_t>(smaller._units.at(idx).value) + carry_over;

            uint32_t const unit_value =
                static_cast<uint32_t>((value < 0 && larger_unit.size > 0) ? larger_unit.size - std::abs(value) : value);
            units.emplace_back(unit{.size = larger_unit.size, .value = unit_value});

            carry_over = (value < 0) ? -1 : 0;
        }

        return number_components{is_lhs_smaller ? rhs._is_minus : this->_is_minus, std::move(units)};
    }
}
