//
//  ae_sheet_content.cpp
//

#include "ae_sheet_content.h"

using namespace yas;
using namespace yas::ae;

time::range const &sheet_content::range_value() const {
    return std::get<time::range>(this->value);
}
