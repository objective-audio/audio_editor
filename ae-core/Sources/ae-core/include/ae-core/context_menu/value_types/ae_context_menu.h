//
//  ae_context_menu.h
//

#pragma once

#include <ae-core/global/value_types/ae_action.h>

#include <optional>
#include <vector>

namespace yas::ae {
struct context_menu final {
    std::string position;
    std::vector<std::optional<ae::action>> actions;
};
}  // namespace yas::ae
