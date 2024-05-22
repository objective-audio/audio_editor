//
//  ae_pasting_module_content.cpp
//

#include "ae_pasting_module_content.hpp"

#include <ae-core/project_editing/value_types/ae_pasting_module.hpp>

using namespace yas;
using namespace yas::ae;

pasting_module_content::pasting_module_content(pasting_module_object const &module, uint32_t const sample_rate,
                                               float const scale)
    : pasting_module_content(module.identifier, module.value.track, module.value.range, sample_rate, scale) {
}

pasting_module_content::pasting_module_content(yas::identifier const &identifier, track_index_t const track,
                                               time::range const &range, uint32_t const sample_rate, float const scale)
    : identifier(identifier), track(track), range(range), sample_rate(sample_rate), scale(scale) {
}

pasting_module_index pasting_module_content::index() const {
    return {this->identifier, this->range, this->track};
}

ui::region pasting_module_content::region() const {
    double const sample_rate = static_cast<double>(this->sample_rate);
    float const x = static_cast<double>(this->range.frame) / sample_rate;
    float const width = static_cast<double>(this->range.length) / sample_rate;
    double const height = 0.8;
    float const y = static_cast<double>(this->track) - height * 0.5f;
    return ui::region{.origin = {.x = x, .y = y}, .size = {.width = width, .height = static_cast<float>(height)}};
}

bool pasting_module_content::operator==(pasting_module_content const &rhs) const {
    return this->identifier == rhs.identifier && this->track == rhs.track && this->range == rhs.range &&
           this->sample_rate == rhs.sample_rate && this->scale == rhs.scale;
}

bool pasting_module_content::operator!=(pasting_module_content const &rhs) const {
    return !(*this == rhs);
}
