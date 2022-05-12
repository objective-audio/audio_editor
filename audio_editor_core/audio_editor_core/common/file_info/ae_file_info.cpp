//
//  ae_file_info.cpp
//

#include "ae_file_info.h"

using namespace yas;
using namespace yas::ae;

bool file_info::operator==(file_info const &rhs) const {
    return this->sample_rate == rhs.sample_rate && this->channel_count == rhs.channel_count &&
           this->length == rhs.length;
}

bool file_info::operator!=(file_info const &rhs) const {
    return !(*this == rhs);
}
