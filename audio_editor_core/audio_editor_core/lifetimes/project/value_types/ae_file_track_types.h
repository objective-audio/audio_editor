//
//  ae_file_track_types.h
//

#pragma once

#include <audio_editor_core/ae_file_module.h>

#include <map>

namespace yas::ae {
using file_track_module_map_t = std::map<time::range, file_module>;

static file_track_module_map_t const empty_file_track_modules;

enum class file_track_event_type {
    any,
    reverted,
    inserted,
    erased,
    detail_updated,
};

struct file_track_event final {
    file_track_event_type type;
    std::optional<file_module> module{std::nullopt};  // inserted, erased
    file_track_module_map_t const &modules;
};
}  // namespace yas::ae
