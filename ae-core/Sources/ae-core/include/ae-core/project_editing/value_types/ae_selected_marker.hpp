//
//  ae_selected_marker.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_common_types.h>

#include <ae-core/project_editing/value_types/ae_marker_index.hpp>

namespace yas::ae {
struct selected_marker final {
    frame_index_t frame;

    explicit selected_marker(frame_index_t const);
};

struct selected_marker_object final {
    object_id identifier;
    selected_marker value;

    selected_marker_object(object_id const &, selected_marker const &);

    [[nodiscard]] marker_index index() const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::selected_marker const &);
}  // namespace yas

std::ostream &operator<<(std::ostream &os, yas::ae::selected_marker const &);
