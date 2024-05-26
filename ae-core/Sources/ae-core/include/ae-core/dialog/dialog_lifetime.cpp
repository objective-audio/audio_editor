//
//  dialog_lifetime.cpp
//

#include "dialog_lifetime.h"

using namespace yas;
using namespace yas::ae;

dialog_lifetime::dialog_lifetime(project_sub_lifetime_id const &lifetime_id, dialog_content const content)
    : lifetime_id(lifetime_id), content(content) {
}
