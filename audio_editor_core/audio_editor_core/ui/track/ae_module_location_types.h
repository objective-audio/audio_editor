//
//  ae_module_location_types.h
//

#pragma once

#include <audio_editor_core/ae_module_location.h>

#include <optional>
#include <vector>

namespace yas::ae {
enum module_location_pool_event_type {
    fetched,
    replaced,
    updated,
};

struct module_location_pool_event {
    module_location_pool_event_type type;
    std::vector<std::optional<module_location>> const &locations;   // fetched, replaced
    std::vector<std::pair<std::size_t, module_location>> inserted;  // updated
    std::vector<std::pair<std::size_t, module_location>> erased;    // updated
};
}  // namespace yas::ae
