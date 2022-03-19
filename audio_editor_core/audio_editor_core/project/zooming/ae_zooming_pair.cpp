//
//  ae_zooming_pair.cpp
//

#include "ae_zooming_pair.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<zooming_pair> zooming_pair::make_shared() {
    return std::shared_ptr<zooming_pair>(new zooming_pair{});
}

zooming_pair::zooming_pair() : horizontal(zooming::make_shared()), vertical(zooming::make_shared()) {
}
