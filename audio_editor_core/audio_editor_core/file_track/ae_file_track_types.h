//
//  ae_file_track_types.h
//

#pragma once

#include <audio_editor_core/ae_file_module.h>

#include <map>

namespace yas::ae {
using file_track_module_map_t = std::map<proc::time::range, file_module>;

enum file_track_event_type {
    any,
    inserted,
    erased,
};

struct file_track_event {
    file_track_event_type type;
    std::optional<file_module> module{std::nullopt};  // inserted, erased
    file_track_module_map_t const &modules;
};
}  // namespace yas::ae
