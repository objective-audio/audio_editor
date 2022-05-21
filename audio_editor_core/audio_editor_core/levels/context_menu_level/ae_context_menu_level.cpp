//
//  ae_context_menu_level.cpp
//

#include "ae_context_menu_level.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<context_menu_level> context_menu_level::make_shared(ae::context_menu const &context_menu) {
    return std::make_shared<context_menu_level>(context_menu);
}

context_menu_level::context_menu_level(ae::context_menu const &context_menu) : context_menu(context_menu) {
}
