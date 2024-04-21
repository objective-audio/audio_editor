//
//  ae_project_format.cpp
//

#include "ae_project_format.h"

using namespace yas;
using namespace yas::ae;

bool project_format::operator==(project_format const &rhs) const {
    return this->sample_rate == rhs.sample_rate && this->channel_count == rhs.channel_count;
}

bool project_format::operator!=(project_format const &rhs) const {
    return !(*this == rhs);
}
