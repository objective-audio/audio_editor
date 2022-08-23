//
//  ae_sheet_content.cpp
//

#include "ae_sheet_content.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<module_name_editor> const &sheet_content::module_name_editor() const {
    return std::get<std::shared_ptr<ae::module_name_editor>>(this->value);
}

int64_t const &sheet_content::integer_value() const {
    return std::get<int64_t>(this->value);
}
