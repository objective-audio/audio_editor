//
//  ae_sheet_lifetime.cpp
//

#include "ae_sheet_lifetime.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<sheet_lifetime> sheet_lifetime::make_shared(sheet_content const &content) {
    return std::make_shared<sheet_lifetime>(content);
}

sheet_lifetime::sheet_lifetime(sheet_content const &content) : content(content) {
}
