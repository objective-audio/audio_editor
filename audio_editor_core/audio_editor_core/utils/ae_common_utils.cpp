//
//  ae_common_utils.cpp
//

#include "ae_common_utils.h"

using namespace yas;
using namespace yas::ae;

std::size_t common_utils::reserving_count(std::size_t const count, std::size_t const interval) {
    if (count > 0) {
        return ((count / interval) + (count % interval ? 1 : 0)) * interval;
    } else {
        return 0;
    }
}
