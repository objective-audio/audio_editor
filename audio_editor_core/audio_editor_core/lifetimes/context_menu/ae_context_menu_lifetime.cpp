//
//  ae_context_menu_lifetime.cpp
//

#include "ae_context_menu_lifetime.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<context_menu_lifetime> context_menu_lifetime::make_shared(project_sub_lifetime_id const &lifetime_id,
                                                                          ae::context_menu const &context_menu) {
    return std::make_shared<context_menu_lifetime>(lifetime_id, context_menu);
}

context_menu_lifetime::context_menu_lifetime(project_sub_lifetime_id const &lifetime_id,
                                             ae::context_menu const &context_menu)
    : lifetime_id(lifetime_id), context_menu(context_menu) {
}
