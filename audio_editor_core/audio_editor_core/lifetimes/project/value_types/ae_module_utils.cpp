//
//  ae_module_utils.cpp
//

#include "ae_module_utils.h"

using namespace yas;
using namespace yas::ae;

bool module_utils::can_split_time_range(time::range const &range, frame_index_t const frame) {
    return range.is_contain(frame) && range.frame != frame;
}
