//
//  ae_module_index.cpp
//

#include "ae_module_index.hpp"

using namespace yas;
using namespace yas::ae;

module_index::module_index(ae::object_id const &object_id, time::range const &range)
    : object_id(object_id), range(range) {
}

bool module_index::operator==(module_index const &rhs) const {
    return this->object_id == rhs.object_id;
}

bool module_index::operator!=(module_index const &rhs) const {
    return !(*this == rhs);
}

bool module_index::operator<(module_index const &rhs) const {
    if (this->range == rhs.range) {
        return this->object_id.identifier() < rhs.object_id.identifier();
    } else {
        return this->range < rhs.range;
    }
}
