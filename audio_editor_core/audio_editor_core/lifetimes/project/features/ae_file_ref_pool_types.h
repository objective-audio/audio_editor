//
//  ae_file_ref_pool_types.h
//

#pragma once

#include <audio_editor_core/ae_file_ref.hpp>
#include <map>

namespace yas::ae {
using file_ref_module_map_t = std::map<std::string, file_ref_object>;

static file_ref_module_map_t const empty_file_refs;

enum class file_ref_pool_event_type {
    any,
    reverted,
    inserted,
    erased,
};

struct file_ref_pool_event final {
    file_ref_pool_event_type type;
    std::optional<file_ref_object> ref{std::nullopt};  // inserted, erased
    file_ref_module_map_t const &refs;
};
}  // namespace yas::ae
