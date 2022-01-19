//
//  ae_action_time_routing.cpp
//

#include "ae_action_time_routing.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_time_routing> action_time_routing::make_shared() {
    return std::shared_ptr<action_time_routing>(new action_time_routing{});
}

action_time_routing::action_time_routing() {
}

std::optional<ae::action> action_time_routing::to_action(ae::key const key) {
    return std::nullopt;
}
