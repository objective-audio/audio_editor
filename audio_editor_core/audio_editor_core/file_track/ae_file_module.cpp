//
//  ae_file_module.cpp
//

#include "ae_file_module.h"

using namespace yas;
using namespace yas::ae;

bool file_module::operator==(file_module const &rhs) const {
    return this->range == rhs.range && this->file_frame == rhs.file_frame;
}

bool file_module::operator!=(file_module const &rhs) const {
    return !(*this == rhs);
}
