//
//  ae_context_menu.h
//

#pragma once

#include <audio_editor_core/ae_action.h>

#include <optional>
#include <vector>

namespace yas::ae {
struct context_menu final {
    std::string position;
    std::vector<std::optional<ae::action>> actions;

    bool operator==(context_menu const &) const;
    bool operator!=(context_menu const &) const;
};
}  // namespace yas::ae
