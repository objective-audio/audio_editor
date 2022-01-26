//
//  ae_index_range.h
//

#pragma once

#include <string>

namespace yas::ae {
struct index_range {
    std::size_t index;
    std::size_t length;

    [[nodiscard]] bool operator==(index_range const &) const;
    [[nodiscard]] bool operator!=(index_range const &) const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::index_range const &);
}

std::ostream &operator<<(std::ostream &os, yas::ae::index_range const &);
