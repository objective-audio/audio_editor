//
//  ae_pasting_module_index.cpp
//

#include "ae_pasting_module_index.hpp"

using namespace yas;
using namespace yas::ae;

pasting_module_index::pasting_module_index(yas::identifier const &identifier, time::range const &range)
    : identifier(identifier), range(range) {
}

bool pasting_module_index::operator==(pasting_module_index const &rhs) const {
    return this->identifier == rhs.identifier;
}

bool pasting_module_index::operator!=(pasting_module_index const &rhs) const {
    return !(*this == rhs);
}

bool pasting_module_index::operator<(pasting_module_index const &rhs) const {
    if (this->range == rhs.range) {
        return this->identifier < rhs.identifier;
    } else {
        return this->range < rhs.range;
    }
}
