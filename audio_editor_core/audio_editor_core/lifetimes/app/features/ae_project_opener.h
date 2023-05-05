//
//  ae_project_opener.h
//

#pragma once

#include <filesystem>
#include <memory>

namespace yas::ae {
class file_info_loader;
class project_lifecycle;
class project_format;

struct project_opener final {
    project_opener(file_info_loader const *, project_lifecycle *);

    void open(project_format const &, std::filesystem::path const &project_path);

   private:
    file_info_loader const *const _file_info_loader;
    project_lifecycle *const _project_lifecycle;

    project_opener(project_opener const &) = delete;
    project_opener(project_opener &&) = delete;
    project_opener &operator=(project_opener const &) = delete;
    project_opener &operator=(project_opener &&) = delete;
};
}  // namespace yas::ae
