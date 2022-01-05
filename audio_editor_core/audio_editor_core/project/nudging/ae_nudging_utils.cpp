//
//  ae_nudging_utils.cpp
//

#include "ae_nudging_utils.h"

using namespace yas;
using namespace yas::ae;

uint32_t nudging_utils::to_sample_count(nudging_kind const kind, uint32_t const sample_rate) {
    switch (kind) {
        case nudging_kind::sample:
            return 1;
        case nudging_kind::milisecond:
            return sample_rate / 1000;
        case nudging_kind::second:
            return sample_rate;
    }
}
