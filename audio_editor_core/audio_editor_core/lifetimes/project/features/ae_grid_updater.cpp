//
//  ae_grid_updater.cpp
//

#include "ae_grid_updater.hpp"

#include <audio_editor_core/ae_nudging.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

grid_updater::grid_updater(timing *timing, nudging *nudging, grid_content_pool *content_pool)
    : _timing(timing), _nudging(nudging), _content_pool(content_pool) {
    timing->observe_fraction_kind([this](auto const &) { this->_update_if_source_is_available(); })
        .end()
        ->add_to(this->_pool);

    nudging->observe_kind([this](auto const &) { this->_update_if_source_is_available(); }).end()->add_to(this->_pool);
}

void grid_updater::update_if_source_changed(source const &source) {
    if (this->_last_source == source) {
        return;
    }

    this->_update(source);
}

void grid_updater::reset() {
    this->_content_pool->update_all({}, false);
    this->_last_source = std::nullopt;
}

void grid_updater::_update(source const &source) {
    auto const sample_rate = this->_timing->sample_rate();

    std::vector<grid_content> result;

    frame_index_t frame = this->_nudging->current_grid_frame(source.begin_frame);

    while (frame < source.end_frame) {
        auto const next_frame = this->_nudging->next_grid_frame(frame);
        if (next_frame - frame < source.min_interval) {
            break;
        }

        frame = next_frame;
        result.emplace_back(grid_content{frame, sample_rate});
    }

    this->_content_pool->update_all(result, false);
    this->_last_source = source;
}

void grid_updater::_update_if_source_is_available() {
    if (this->_last_source.has_value()) {
        this->_update(this->_last_source.value());
    }
}

#pragma mark - source

bool grid_updater::source::operator==(source const &rhs) const {
    return this->begin_frame == rhs.begin_frame && this->end_frame == rhs.end_frame &&
           this->min_interval == rhs.min_interval;
}

bool grid_updater::source::operator!=(source const &rhs) const {
    return !(*this == rhs);
}
