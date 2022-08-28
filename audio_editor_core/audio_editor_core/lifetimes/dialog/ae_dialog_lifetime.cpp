//
//  ae_dialog_lifetime.cpp
//

#include "ae_dialog_lifetime.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<dialog_lifetime> dialog_lifetime::make_shared(project_sub_lifetime_id const &lifetime_id,
                                                              dialog_content const content) {
    return std::make_shared<dialog_lifetime>(lifetime_id, content);
}

dialog_lifetime::dialog_lifetime(project_sub_lifetime_id const &lifetime_id, dialog_content const content)
    : lifetime_id(lifetime_id), content(content) {
}
