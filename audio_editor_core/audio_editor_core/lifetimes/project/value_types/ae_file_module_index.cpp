//
//  ae_file_module_index.cpp
//

#include "ae_file_module_index.hpp"

using namespace yas;
using namespace yas::ae;

file_module_index::file_module_index(ae::object_id const &object_id, time::range const &range)
    : object_id(object_id), range(range) {
}

bool file_module_index::operator==(file_module_index const &rhs) const {
    return this->object_id == rhs.object_id;
}

bool file_module_index::operator!=(file_module_index const &rhs) const {
    return !(*this == rhs);
}

bool file_module_index::operator<(file_module_index const &rhs) const {
    if (this->range == rhs.range) {
        return this->object_id.identifier() < rhs.object_id.identifier();
    } else {
        return this->range < rhs.range;
    }
}
