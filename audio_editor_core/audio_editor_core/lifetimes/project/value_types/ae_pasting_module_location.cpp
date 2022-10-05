//
//  ae_pasting_module_location.cpp
//

#include "ae_pasting_module_location.hpp"

#include <audio_editor_core/ae_pasting_file_module.hpp>

using namespace yas;
using namespace yas::ae;

pasting_module_location::pasting_module_location(pasting_file_module_object const &module, uint32_t const sample_rate,
                                                 float const scale)
    : pasting_module_location(module.identifier, module.value.range, sample_rate, scale) {
}

pasting_module_location::pasting_module_location(yas::identifier const &identifier, time::range const &range,
                                                 uint32_t const sample_rate, float const scale)
    : identifier(identifier), range(range), sample_rate(sample_rate), scale(scale) {
}

pasting_file_module_index pasting_module_location::index() const {
    return {this->identifier, this->range};
}

float pasting_module_location::x() const {
    return static_cast<double>(this->range.frame) / static_cast<double>(this->sample_rate);
}

float pasting_module_location::width() const {
    return static_cast<double>(this->range.length) / static_cast<double>(this->sample_rate);
}

bool pasting_module_location::operator==(pasting_module_location const &rhs) const {
    return this->identifier == rhs.identifier && this->range == rhs.range && this->sample_rate == rhs.sample_rate &&
           this->scale == rhs.scale;
}

bool pasting_module_location::operator!=(pasting_module_location const &rhs) const {
    return !(*this == rhs);
}
