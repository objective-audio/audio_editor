//
//  action_receiver_providable.cpp
//

#include "action_receiver_providable.h"

#include <ae-core/global/value_types/action_id.h>
#include <cpp-utils/stl_utils.h>

#include "action_receivable.h"

using namespace yas;
using namespace yas::ae;

std::vector<action_receiver_providable const *> action_receiver_providable::providers(
    std::optional<action_id> const &action_id) const {
    if (is_match(action_id, this->receivable_id())) {
        std::vector<action_receiver_providable const *> result;

        for (auto const &sub_provider : this->sub_providers()) {
            move_back_insert(result, sub_provider->providers(action_id));
        }

        result.emplace_back(this);

        return result;
    } else {
        return {};
    }
}

std::vector<action_receivable const *> action_receiver_providable::receivers(
    std::optional<action_id> const &action_id) const {
    std::vector<action_receivable const *> result;

    for (auto const &provider : this->providers(action_id)) {
        for (auto const &receiver : provider->receivers()) {
            if (is_match(action_id, receiver->receivable_id())) {
                result.emplace_back(receiver);
            }
        }
    }

    return result;
}
