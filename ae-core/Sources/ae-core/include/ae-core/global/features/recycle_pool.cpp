//
//  recycle_pool.cpp
//

#include "recycle_pool.h"

using namespace yas;
using namespace yas::ae;

std::string yas::to_string(ae::recycle_pool_event_type const &type) {
    switch (type) {
        case ae::recycle_pool_event_type::fetched:
            return "fetched";
        case ae::recycle_pool_event_type::updated:
            return "updated";
        case ae::recycle_pool_event_type::replaced:
            return "replaced";
    }
}

std::ostream &operator<<(std::ostream &os, yas::ae::recycle_pool_event_type const &value) {
    os << yas::to_string(value);
    return os;
}
