//
//  number_components.h
//  audio_editor_core
//
//  Created by Yuki Yasoshima on 2022/01/15.
//

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace yas::ae {
struct number_components_unit final {
    std::size_t const size;
    uint32_t value = 0;

    bool operator==(number_components_unit const &rhs) const;
    bool operator!=(number_components_unit const &rhs) const;
};

struct number_components final {
    number_components(bool const is_minus, std::vector<number_components_unit> &&units);

    [[nodiscard]] bool is_minus() const;

    [[nodiscard]] std::vector<number_components_unit> const &units() const;
    [[nodiscard]] std::size_t size() const;

    [[nodiscard]] number_components_unit const &unit(std::size_t const) const;

    bool is_zero() const;
    number_components abs() const;

    bool is_equal_structure(number_components const &) const;

    bool operator==(number_components const &rhs) const;
    bool operator!=(number_components const &rhs) const;
    bool operator<(number_components const &rhs) const;

    number_components is_minus_replaced(bool const) const;
    number_components unit_value_replaced(uint32_t const value, std::size_t const idx) const;
    number_components adding(number_components const &) const;

   private:
    bool _is_minus = false;
    std::vector<number_components_unit> _units;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::number_components const &);
}

std::ostream &operator<<(std::ostream &os, yas::ae::number_components const &);
