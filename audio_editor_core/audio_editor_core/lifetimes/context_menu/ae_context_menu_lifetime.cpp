//
//  ae_context_menu_lifetime.cpp
//

#include "ae_context_menu_lifetime.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<context_menu_lifetime> context_menu_lifetime::make_shared(ae::context_menu const &context_menu) {
    return std::make_shared<context_menu_lifetime>(context_menu);
}

context_menu_lifetime::context_menu_lifetime(ae::context_menu const &context_menu) : context_menu(context_menu) {
}
