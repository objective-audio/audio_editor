//
//  ae_nudging.cpp
//

#include "ae_nudging.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<nudging> nudging::make_shared() {
    return std::shared_ptr<nudging>(new nudging{});
}

nudging::nudging() : _kind(nudging_kind::sample) {
}

void nudging::set_kind(nudging_kind const kind) {
    this->_kind = kind;
}

nudging_kind nudging::kind() const {
    return this->_kind;
}
