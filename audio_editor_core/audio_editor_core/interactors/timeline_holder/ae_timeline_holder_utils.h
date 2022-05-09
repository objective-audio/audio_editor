//
//  ae_timeline_holder_utils.h
//

#pragma once

#include <audio_editor_core/ae_file_module.h>
#include <cpp_utils/yas_url.h>
#include <processing/yas_processing_umbrella.h>

namespace yas::ae::timeline_holder_utils {
proc::module_ptr make_module(file_module const &file_module, url const &url, uint32_t const ch_count);
}
