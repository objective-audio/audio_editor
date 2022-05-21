//
//  ae_sheet_level.cpp
//

#include "ae_sheet_level.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<sheet_level> sheet_level::make_shared(sheet_content const &content) {
    return std::make_shared<sheet_level>(content);
}

sheet_level::sheet_level(sheet_content const &content) : content(content) {
}
