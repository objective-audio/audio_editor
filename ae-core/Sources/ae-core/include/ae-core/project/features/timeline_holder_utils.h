//
//  timeline_holder_utils.h
//

#pragma once

#include <ae-core/project_editing/value_types/module.h>

#include <audio-processing/umbrella.hpp>

namespace yas::ae {
class project_path;
}

namespace yas::ae::timeline_holder_utils {
proc::module_ptr make_module(module const &, project_path const *, uint32_t const ch_count);
}
