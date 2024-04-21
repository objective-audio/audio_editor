//
//  ae_timing_types.h
//

#pragma once

#include <ae-core/global/value_types/ae_common_types.h>
#include <ae-core/global/value_types/ae_number_components.h>

#include <string>

namespace yas::ae {
enum class timing_fraction_kind {
    sample,
    milisecond,
    frame30,
};

enum class timing_unit_kind : std::size_t {
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
        std::size_t fraction_unit_size;
    };

    timing_components(number_components const &);
    timing_components(args const &);

    [[nodiscard]] number_components const &raw_components() const;

    [[nodiscard]] bool is_minus() const;
    [[nodiscard]] uint32_t value(timing_unit_kind const) const;
    [[nodiscard]] std::size_t fraction_unit_size() const;

    [[nodiscard]] bool is_zero() const;
    [[nodiscard]] timing_components abs() const;

    bool operator==(timing_components const &rhs) const;
    bool operator!=(timing_components const &rhs) const;
    bool operator<(timing_components const &) const;

    [[nodiscard]] timing_components adding(timing_components const &) const;

    struct offset_args {
        bool is_minus;
        uint32_t count;
        std::size_t unit_index;
        std::size_t fraction_unit_size;
    };

    [[nodiscard]] static timing_components offset(offset_args &&args);

   private:
    number_components _components;
};
}  // namespace yas::ae

namespace yas {
ae::timing_fraction_kind rotate_next(ae::timing_fraction_kind const);

std::size_t to_index(ae::timing_unit_kind const);
ae::timing_unit_kind rotate_next(ae::timing_unit_kind const);
ae::timing_unit_kind rotate_previous(ae::timing_unit_kind const);

std::string to_string(ae::timing_components const &);
}  // namespace yas

std::ostream &operator<<(std::ostream &, yas::ae::timing_components const &);
