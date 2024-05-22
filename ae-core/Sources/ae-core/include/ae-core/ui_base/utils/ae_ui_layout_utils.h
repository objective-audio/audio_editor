//
//  ae_ui_layout_utils.h
//

#pragma once

#include <ui/common/yas_ui_types.h>

namespace yas::ae::ui_layout_utils {
std::function<float(float const &)> constant(float const distance);
std::function<ui::point(ui::point const &)> constant(ui::point const &distance);
std::function<ui::range(ui::range const &)> constant(ui::range_insets const &distance);
std::function<ui::region(ui::region const &)> constant(ui::region_insets const &insets);
}  // namespace yas::ae::ui_layout_utils
