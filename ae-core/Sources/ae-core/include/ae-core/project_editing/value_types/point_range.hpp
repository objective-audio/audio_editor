//
//  point_range.hpp
//

#pragma once

#include <ui/common/yas_ui_types.h>

namespace yas::ae {
struct point_range {
    ui::point first;
    ui::point second;

    [[nodiscard]] ui::region region() const;

    [[nodiscard]] point_range second_updated(ui::point const &) const;

    bool operator==(point_range const &) const;
    bool operator!=(point_range const &) const;
};
}  // namespace yas::ae
