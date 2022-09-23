//
//  ae_timeline_holder_utils.cpp
//

#include "ae_timeline_holder_utils.h"

#include <audio_editor_core/ae_project_path.h>

using namespace yas;
using namespace yas::ae;

proc::module_ptr timeline_holder_utils::make_module(file_module const &file_module, project_path const *project_path,
                                                    uint32_t const ch_count) {
    auto const path = project_path->editing_files_directory().append(file_module.file_name);
    auto const module = proc::file::make_signal_module<float>(path, file_module.range.frame, file_module.file_frame);

    auto each = make_fast_each(ch_count);
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        module->connect_output(idx, idx);
    }

    return module;
}
