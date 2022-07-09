//
//  ae_project_launch_lifetime.cpp
//

#include "ae_project_launch_lifetime.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_launch_lifetime> project_launch_lifetime::make_shared(ae::window_lifetime_id const &lifetime) {
    return std::make_shared<project_launch_lifetime>(lifetime);
}

project_launch_lifetime::project_launch_lifetime(ae::window_lifetime_id const &lifetime)
    : window_lifetime_id(lifetime) {
}
