//
//  timeline_holder_utils.cpp
//

#include "timeline_holder_utils.h"

#include <ae-core/project/features/project_path.h>

using namespace yas;
using namespace yas::ae;

proc::module_ptr timeline_holder_utils::make_module(module const &module, project_path const *project_path,
                                                    uint32_t const ch_count) {
    auto const path = project_path->editing_files_directory().append(module.file_name);
    auto const signal_module = proc::file::make_signal_module<float>(path, module.range.frame, module.file_frame);

    auto each = make_fast_each(ch_count);
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        signal_module->connect_output(idx, idx);
    }

    return signal_module;
}
