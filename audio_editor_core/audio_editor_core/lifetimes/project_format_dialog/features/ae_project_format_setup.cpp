//
//  ae_project_format_setup.cpp
//

#include "ae_project_format_setup.h"

#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

project_format_setup::project_format_setup()
    : available_sample_rates({96000, 48000, 44100, 22050}),
      min_channel_count(1),
      max_channel_count(64),
      _sample_rate_idx(observing::value::holder<std::size_t>::make_shared(1)),
      _channel_count(observing::value::holder<uint32_t>::make_shared(2)) {
}

std::size_t const &project_format_setup::sample_rate_index() const {
    return this->_sample_rate_idx->value();
}

void project_format_setup::set_sample_rate_index(std::size_t const idx) {
    this->_sample_rate_idx->set_value(idx);
}

uint32_t const &project_format_setup::channel_count() const {
    return this->_channel_count->value();
}

void project_format_setup::set_channel_count(uint32_t const count) {
    if (count < this->min_channel_count || this->max_channel_count < count) {
        assertion_failure_if_not_test();
        return;
    }
    this->_channel_count->set_value(count);
}

project_format project_format_setup::format() const {
    return {.sample_rate = this->available_sample_rates.at(this->_sample_rate_idx->value()),
            .channel_count = this->_channel_count->value()};
}

observing::syncable project_format_setup::observe_sample_rate_index(
    std::function<void(std::size_t const &)> &&handler) {
    return this->_sample_rate_idx->observe(std::move(handler));
}

observing::syncable project_format_setup::observe_channel_count(std::function<void(uint32_t const &)> &&handler) {
    return this->_channel_count->observe(std::move(handler));
}
