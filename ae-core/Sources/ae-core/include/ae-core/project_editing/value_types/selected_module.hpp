//
//  selected_module.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

#include <ae-core/project_editing/value_types/module_index.hpp>

namespace yas::ae {
struct selected_module final {
    time::range range;
    track_index_t track;

    selected_module(time::range const &, track_index_t const);
};

struct selected_module_object final {
    object_id identifier;
    selected_module value;

    selected_module_object(object_id const &, selected_module const &);

    [[nodiscard]] module_index index() const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::selected_module const &);
}  // namespace yas

std::ostream &operator<<(std::ostream &os, yas::ae::selected_module const &);
