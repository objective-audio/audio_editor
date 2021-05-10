//
//  ae_project_editor_utils.cpp
//

#include "ae_project_editor_utils.h"

using namespace yas;
using namespace yas::ae;

proc::module_ptr project_editor_utils::make_module(file_module const &file_module, url const &url,
                                                   uint32_t const ch_count) {
    auto const module = proc::file::make_signal_module<float>(url, file_module.range.frame, file_module.file_frame);

    auto each = make_fast_each(ch_count);
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        module->connect_output(idx, idx);
    }

    return module;
}
