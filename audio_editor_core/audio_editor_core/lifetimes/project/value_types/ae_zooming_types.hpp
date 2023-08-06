//
//  ae_zooming_types.hpp
//

#pragma once

namespace yas::ae {
struct zooming_size final {
    double width_per_sec;
    double height_per_track;
};

struct zooming_scale final {
    double horizontal = 1.0;
    double vertical = 1.0;
};
}  // namespace yas::ae

namespace yas {
[[nodiscard]] ae::zooming_size to_zooming_size(ae::zooming_scale const &);
}
