//
//  file_importer_types.h
//

#pragma once

#include <ae-core/app/value_types/project_format.h>
#include <ae-core/global/value_types/project_guid.h>

#include <filesystem>
#include <functional>

namespace yas::ae {
struct file_importing_context final {
    project_guid project_id;
    std::filesystem::path src_path;
    std::filesystem::path dst_path;
    project_format project_format;
    std::function<void(bool const)> completion;
};
}  // namespace yas::ae
