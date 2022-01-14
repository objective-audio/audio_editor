//
//  ae_nudging_utils.cpp
//

#include "ae_nudging_utils.h"

using namespace yas;
using namespace yas::ae;

timing_components nudging_utils::offset_components(bool const is_previous, nudging_kind const nudging_kind) {
    switch (nudging_kind) {
        case nudging_kind::minute:
            return {.is_minus = is_previous, .minutes = 1};
        case nudging_kind::second:
            return {.is_minus = is_previous, .seconds = 1};
        case nudging_kind::fraction:
            return {.is_minus = is_previous, .fraction = 1};
    }
}
