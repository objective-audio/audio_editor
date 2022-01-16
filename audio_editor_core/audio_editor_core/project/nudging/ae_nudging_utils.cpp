//
//  ae_nudging_utils.cpp
//

#include "ae_nudging_utils.h"

using namespace yas;
using namespace yas::ae;

timing_components nudging_utils::offset_components(bool const is_previous, uint32_t const count,
                                                   nudging_kind const nudging_kind,
                                                   uint32_t const fraction_unit_count) {
    switch (nudging_kind) {
        case nudging_kind::minute:
            return {{.is_minus = is_previous,
                     .minutes = static_cast<uint8_t>(count),
                     .fraction_unit_count = fraction_unit_count}};
        case nudging_kind::second:
            return {{.is_minus = is_previous,
                     .seconds = static_cast<uint8_t>(count),
                     .fraction_unit_count = fraction_unit_count}};
        case nudging_kind::fraction:
            return {{.is_minus = is_previous, .fraction = count, .fraction_unit_count = fraction_unit_count}};
    }
}
