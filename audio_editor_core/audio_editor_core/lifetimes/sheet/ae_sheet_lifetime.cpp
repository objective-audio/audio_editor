//
//  ae_sheet_lifetime.cpp
//

#include "ae_sheet_lifetime.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<sheet_lifetime> sheet_lifetime::make_shared(sheet_lifetime_id const &lifetime_id,
                                                            sheet_content const &content) {
    return std::make_shared<sheet_lifetime>(lifetime_id, content);
}

sheet_lifetime::sheet_lifetime(sheet_lifetime_id const &lifetime_id, sheet_content const &content)
    : lifetime_id(lifetime_id), content(content) {
}
