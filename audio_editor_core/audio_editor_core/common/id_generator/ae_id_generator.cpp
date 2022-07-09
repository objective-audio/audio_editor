//
//  ae_id_generator.cpp
//

#include "ae_id_generator.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<id_generator> id_generator::make_shared() {
    return std::make_shared<id_generator>();
}

identifier id_generator::generate() const {
    return {};
}
