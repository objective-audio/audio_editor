//
//  ae_file_importer_types.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_url.h>

#include <functional>

namespace yas::ae {
struct file_importing_context final {
    project_id project_id;
    url src_url;
    url dst_url;
    std::function<void(bool const)> completion;
};
}  // namespace yas::ae
