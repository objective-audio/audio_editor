//
//  ae_window_opener.h
//

#pragma once

#include <filesystem>
#include <memory>

namespace yas::ae {
class file_info_loader;
class window_lifecycle;
class project_format;

struct window_opener final {
    window_opener(file_info_loader const *, window_lifecycle *);

    void open(project_format const &, std::filesystem::path const &project_path);

   private:
    file_info_loader const *const _file_info_loader;
    window_lifecycle *const _window_lifecycle;

    window_opener(window_opener const &) = delete;
    window_opener(window_opener &&) = delete;
    window_opener &operator=(window_opener const &) = delete;
    window_opener &operator=(window_opener &&) = delete;
};
}  // namespace yas::ae
