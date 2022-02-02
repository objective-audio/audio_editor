//
//  ae_nudging_utils.cpp
//

#include "ae_nudging_utils.h"

using namespace yas;
using namespace yas::ae;

timing_components nudging_utils::offset_components(bool const is_previous, uint32_t const count,
                                                   std::size_t const unit_idx, std::size_t const fraction_unit_size) {
    return {{.is_minus = is_previous,
             .fraction = static_cast<uint32_t>((unit_idx == 0) ? count : 0),
             .seconds = static_cast<uint8_t>((unit_idx == 1) ? count : 0),
             .minutes = static_cast<uint8_t>((unit_idx == 2) ? count : 0),
             .hours = static_cast<uint8_t>((unit_idx == 3) ? count : 0),
             .fraction_unit_size = fraction_unit_size}};
}
