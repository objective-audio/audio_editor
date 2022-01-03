//
//  ae_nudging.cpp
//

#include "ae_nudging.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<nudging> nudging::make_shared() {
    return std::shared_ptr<nudging>(new nudging{});
}

nudging::nudging() {
}
