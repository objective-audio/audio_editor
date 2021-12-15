//
//  ae_module_location.cpp
//

#include "ae_module_location.h"

using namespace yas;
using namespace yas::ae;

module_location module_location::make(ae::identifier const &identifier, proc::time::range const &range,
                                      uint32_t const sample_rate) {
    float const x = static_cast<double>(range.frame) / static_cast<double>(sample_rate);
    float const width = static_cast<double>(range.length) / static_cast<double>(sample_rate);
    return module_location{identifier, x, width, range};
}
