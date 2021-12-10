//
//  ae_display_space.mm
//

#include "ae_display_space.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<display_space> display_space::make_shared(ui::region const view_region) {
    return std::shared_ptr<display_space>(new display_space{view_region});
}

display_space::display_space(ui::region const view_region)
    : _view_region(view_region),
      _scale({1.0f, 1.0f}),
      _region(observing::value::holder<ui::region>::make_shared(ui::region::zero())) {
    this->_update();
}

void display_space::set_view_region(ui::region const view_region) {
    this->_view_region = view_region;
    this->_update();
}

void display_space::set_scale(ui::size const scale) {
    this->_scale = scale;
    this->_update();
}

ui::region display_space::region() const {
    return this->_region->value();
}

observing::syncable display_space::observe_region(std::function<void(ui::region const &)> &&handler) {
    return this->_region->observe(std::move(handler));
}

proc::time::range display_space::frame_range(uint32_t const sample_rate,
                                             proc::frame_index_t const current_frame) const {
    auto const &region = this->region();
    auto const min_edge_frame =
        current_frame + static_cast<proc::frame_index_t>(std::floor(region.left() * sample_rate));
    auto const max_edge_frame =
        current_frame + static_cast<proc::frame_index_t>(std::ceil(region.right() * sample_rate));
    return proc::time::range{min_edge_frame, static_cast<proc::length_t>(max_edge_frame - min_edge_frame)};
}

void display_space::_update() {
    if (this->_scale.width == 0 || this->_scale.height == 0) {
        return;
    }

    auto const x_scale = 1.0f / this->_scale.width;
    auto const y_scale = 1.0f / this->_scale.height;
    auto const insets = this->_view_region.insets();

    auto const left = insets.left * x_scale;
    auto const right = insets.right * x_scale;
    auto const top = insets.top * y_scale;
    auto const bottom = insets.bottom * y_scale;

    this->_region->set_value(
        ui::region{.origin = {.x = left, .y = bottom}, .size = {.width = right - left, .height = top - bottom}});
}
