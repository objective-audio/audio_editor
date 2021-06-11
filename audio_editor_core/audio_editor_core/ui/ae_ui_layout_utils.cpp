//
//  ae_ui_layout_utils.cpp
//

#include "ae_ui_layout_utils.h"

using namespace yas;
using namespace yas::ui;
using namespace yas::ae;

std::function<float(float const &)> ui_layout_utils::constant(float const distance) {
    return [distance](float const &value) { return value + distance; };
}

std::function<ui::point(ui::point const &)> ui_layout_utils::constant(ui::point const &distance) {
    return [distance](ui::point const &point) { return point + distance; };
}

std::function<ui::range(ui::range const &)> ui_layout_utils::constant(ui::range_insets const &distance) {
    return [distance](ui::range const &range) { return range + distance; };
}

std::function<ui::region(ui::region const &)> ui_layout_utils::constant(ui::region_insets const &insets) {
    return [insets](ui::region const &region) { return region + insets; };
}
