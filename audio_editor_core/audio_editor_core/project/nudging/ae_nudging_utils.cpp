//
//  ae_nudging_utils.cpp
//

#include "ae_nudging_utils.h"

using namespace yas;
using namespace yas::ae;

uint32_t nudging_utils::to_sample_count(nudging_kind const kind, std::shared_ptr<timing_for_nudging> const &timing) {
    switch (kind) {
        case nudging_kind::fragment:
            return timing->unit_sample_count();
        case nudging_kind::second:
            return timing->sample_rate();
        case nudging_kind::minute:
            return timing->sample_rate() * 60;
    }
}
