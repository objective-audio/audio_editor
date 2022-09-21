//
//  ae_timeline_holder_utils.h
//

#pragma once

#include <audio_editor_core/ae_file_module.h>
#include <processing/yas_processing_umbrella.h>

namespace yas::ae {
class project_path;
}

namespace yas::ae::timeline_holder_utils {
proc::module_ptr make_module(file_module_object const &file_module, project_path const *, uint32_t const ch_count);
}
