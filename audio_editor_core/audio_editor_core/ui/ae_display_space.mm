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
    : _region(ui::region::zero()), _view_region(view_region), _scale({1.0f, 1.0f}) {
    this->_fetcher = observing::fetcher<display_space_event>::make_shared([this] {
        return display_space_event{
            .source = display_space_event_source::fetched, .region = this->_region, .scale = this->_scale};
    });
    this->_update_region_and_notify(display_space_event_source::fetched);
}

void display_space::set_view_region(ui::region const view_region) {
    this->_view_region = view_region;
    this->_update_region_and_notify(display_space_event_source::view);
}

void display_space::set_scale(ui::size const scale) {
    this->_scale = scale;
    this->_update_region_and_notify(display_space_event_source::scale);
}

ui::region display_space::region() const {
    return this->_region;
}

observing::syncable display_space::observe(std::function<void(display_space_event const &)> &&handler) {
    return this->_fetcher->observe(std::move(handler));
}

ui::size const &display_space::scale() const {
    return this->_scale;
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

void display_space::_update_region_and_notify(display_space_event_source const source) {
    ui::region const prev_region = this->_region;

    if (this->_scale.width == 0 || this->_scale.height == 0) {
        this->_region = ui::region::zero();
    } else {
        auto const x_scale = 1.0f / this->_scale.width;
        auto const y_scale = 1.0f / this->_scale.height;
        auto const insets = this->_view_region.insets();

        auto const left = insets.left * x_scale;
        auto const right = insets.right * x_scale;
        auto const top = insets.top * y_scale;
        auto const bottom = insets.bottom * y_scale;

        this->_region = {.origin = {.x = left, .y = bottom}, .size = {.width = right - left, .height = top - bottom}};
    }

    if (prev_region != this->_region) {
        this->_fetcher->push({.source = source, .region = this->_region, .scale = this->_scale});
    }
}
