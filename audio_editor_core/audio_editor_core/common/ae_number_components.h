//
//  ae_number_components.h
//  audio_editor_core
//
//  Created by Yuki Yasoshima on 2022/01/15.
//

#pragma once

#include <memory>
#include <vector>

namespace yas::ae {
struct number_components final {
    struct unit final {
        uint32_t value = 0;
        uint32_t const count;

        bool operator==(unit const &rhs) const;
        bool operator!=(unit const &rhs) const;
    };

    number_components(bool const is_minus, std::vector<unit> &&units);

    void set_is_minus(bool const);
    [[nodiscard]] bool is_minus() const;

    [[nodiscard]] std::size_t units_count() const;

    void set_unit_value(uint32_t const value, std::size_t const idx);
    [[nodiscard]] uint32_t unit_value(std::size_t const) const;

    bool is_zero() const;
    number_components abs() const;

    bool is_equal_structure(number_components const &) const;

    bool operator==(number_components const &rhs) const;
    bool operator!=(number_components const &rhs) const;
    bool operator<(number_components const &rhs) const;

    number_components adding(number_components const &) const;

   private:
    bool _is_minus = false;
    std::vector<unit> _units;
};
}  // namespace yas::ae
