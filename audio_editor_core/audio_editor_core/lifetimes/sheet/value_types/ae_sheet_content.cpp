//
//  ae_sheet_content.cpp
//

#include "ae_sheet_content.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<module_name_editor> const &sheet_content::module_name_editor() const {
    return std::get<std::shared_ptr<ae::module_name_editor>>(this->value);
}

std::shared_ptr<marker_name_editor> const &sheet_content::marker_name_editor() const {
    return std::get<std::shared_ptr<ae::marker_name_editor>>(this->value);
}
